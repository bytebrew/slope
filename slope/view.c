/*
 * Copyright (C) 2015  Elvis Teixeira
 *
 * This source code is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "slope/view.h"


#define SLOPE_VIEW_GET_PRIVATE(obj)     \
  (G_TYPE_INSTANCE_GET_PRIVATE((obj),   \
   SLOPE_VIEW_TYPE, SlopeViewPrivate))


/**
 */
static gboolean
_on_draw_event (GtkWidget *widget, cairo_t *cr, gpointer *data);


/**
 */
static gboolean
_on_button_press_event (GtkWidget *widget, GdkEventButton *event,
                        gpointer *data);


/**
 */
static gboolean
_on_button_move_event (GtkWidget *widget, GdkEventButton *event,
                       gpointer *data);


/**
 */
static gboolean
_on_button_release_event (GtkWidget *widget, GdkEventButton *event,
                          gpointer *data);


/**
 *
 */
static void
_view_destroy (GtkWidget *widget);


/**
 */
typedef struct _SlopeViewPrivate SlopeViewPrivate;


/**
 */
struct _SlopeViewPrivate
{
  slope_figure_t *figure;
  cairo_surface_t *back_surf;
  slope_point_t move_start;
  slope_point_t move_end;
  slope_color_t mouse_rec_color;
  slope_bool_t mouse_zoom;
  slope_bool_t own_figure;
  int on_move;
  int press_sig_id;
  int move_sig_id;
  int release_sig_id;
};


G_DEFINE_TYPE(SlopeView, slope_view, GTK_TYPE_DRAWING_AREA);


static void
slope_view_class_init (SlopeViewClass *klass)
{
  GtkWidgetClass *widget_klass = GTK_WIDGET_CLASS (klass);

  g_type_class_add_private(klass, sizeof(SlopeViewPrivate));

  widget_klass->destroy = _view_destroy;
}


static void
slope_view_init (SlopeView *view)
{
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE (view);
  GtkWidget *widget = GTK_WIDGET (view);

  priv->on_move = SLOPE_FALSE;
  priv->mouse_zoom = SLOPE_FALSE;
  slope_color_set_name(&priv->mouse_rec_color, SLOPE_BLACK);

  gtk_widget_add_events(widget,
      GDK_EXPOSURE_MASK
      |GDK_BUTTON_MOTION_MASK
      |GDK_BUTTON_PRESS_MASK
      |GDK_BUTTON_RELEASE_MASK);

  g_signal_connect(G_OBJECT(view), "draw", G_CALLBACK(_on_draw_event), NULL);
  slope_view_toggle_mouse_zoom(widget, SLOPE_TRUE);
}


GtkWidget *
slope_view_new ()
{
  GtkWidget *view = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE (view);
  priv->figure = slope_figure_create();
  priv->own_figure = SLOPE_TRUE;
  return view;
}


GtkWidget *
slope_view_new_for_figure (slope_figure_t *figure, slope_bool_t own_figure)
{
  GtkWidget *view = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE (view);
  priv->figure = figure;
  priv->own_figure = own_figure;
  return view;
}


slope_figure_t*
slope_view_get_figure (GtkWidget *view)
{
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE (view);

  if (priv != NULL) {
    return priv->figure;
  }
  return NULL;
}


static gboolean
_on_draw_event (GtkWidget *widget, cairo_t *cr, gpointer *data)
{
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE (widget);
  int width, height;
  slope_rect_t rect;
  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);

  /* TODO: save figure in back_surf instead of recalculate
   * everithing for each zooming redraw
   */
  slope_rect_set(&rect, 0.0, 0.0, (double)width, (double)height);
  slope_figure_t *figure = priv->figure;
  slope_figure_draw(figure, cr, &rect);

  if (priv->on_move) {
    slope_cairo_set_color(cr, &priv->mouse_rec_color);
    cairo_set_line_width(cr, 1.0);
    double dash[2] = { 3.0, 3.0 };
    cairo_set_dash(cr, dash, 2, 0.0);
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
    cairo_rectangle(cr, priv->move_start.x, priv->move_start.y,
        priv->move_end.x - priv->move_start.x,
        priv->move_end.y - priv->move_start.y);
    cairo_stroke(cr);
  }
  return TRUE;
}


static gboolean
_on_button_press_event (GtkWidget *widget, GdkEventButton *event,
                       gpointer *data)
{
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(widget);

  if (event->button == 1 /*left button*/) {
    priv->move_start.x = event->x;
    priv->move_start.y = event->y;
    priv->move_end.x = event->x;
    priv->move_end.y = event->y;
    priv->on_move = SLOPE_TRUE;
  }
  else if (event->button == 3 /*right button*/) {
    slope_figure_update(priv->figure);
    gtk_widget_queue_draw(widget);
  }
  return TRUE;
}


static gboolean
_on_button_move_event (GtkWidget *widget, GdkEventButton *event,
                               gpointer *data)
{
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(widget);

  if (priv->on_move) {
    priv->move_end.x = event->x;
    priv->move_end.y = event->y;
    gtk_widget_queue_draw(widget);
  }
  return TRUE;
}


static gboolean
_on_button_release_event (GtkWidget *widget, GdkEventButton *event,
                          gpointer *data)
{
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(widget);

  if (priv->on_move) {
    priv->on_move = SLOPE_FALSE;
    priv->move_end.x = event->x;
    priv->move_end.y = event->y;

    /* if the region is too small, the user probably just
       clicked on a point, no region to track */
    double width = priv->move_start.x - priv->move_end.x;
    double height = priv->move_start.y - priv->move_end.y;
    if (width < 0.0) width = -width;
    if (width < 3.0) return TRUE;
    if (height < 0.0) height = -height;
    if (height < 3.0) return TRUE;

    /* if a good region was selected, let's track it! */
    slope_figure_track_region(
        priv->figure,
        priv->move_start.x, priv->move_start.y,
        priv->move_end.x, priv->move_end.y);

    gtk_widget_queue_draw(widget);
  }
  return TRUE;
}


void
slope_view_toggle_mouse_zoom (GtkWidget *view, slope_bool_t on)
{
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(view);

  if (on == SLOPE_TRUE && priv->mouse_zoom == SLOPE_FALSE) {
    priv->press_sig_id =
      g_signal_connect(G_OBJECT(view), "button-press-event",
          G_CALLBACK(_on_button_press_event), NULL);
    priv->move_sig_id =
      g_signal_connect(G_OBJECT(view), "motion-notify-event",
          G_CALLBACK(_on_button_move_event), NULL);
    priv->release_sig_id =
      g_signal_connect(G_OBJECT(view), "button-release-event",
          G_CALLBACK(_on_button_release_event), NULL);
    priv->mouse_zoom = SLOPE_TRUE;
  } else if (on == SLOPE_FALSE && priv->mouse_zoom == SLOPE_TRUE) {
    g_signal_handler_disconnect(view, priv->press_sig_id);
    g_signal_handler_disconnect(view, priv->move_sig_id);
    g_signal_handler_disconnect(view, priv->release_sig_id);
    priv->mouse_zoom = SLOPE_FALSE;
  }
}


static void
_view_destroy (GtkWidget *widget)
{
  GtkWidgetClass *widget_klass = GTK_WIDGET_GET_CLASS (widget);
  SlopeView *view = SLOPE_VIEW (widget);
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE (view);
 
  /*
  if (priv->own_figure) {
    puts("cleaning");
    slope_figure_destroy(priv->figure);
    priv->own_figure = SLOPE_FALSE;
  }
 
   widget_klass->destroy(widget);
   
   Why does it break? */
}

/* slope/view.c */
