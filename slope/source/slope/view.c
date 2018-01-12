/*
 * Copyright (C) 2017  Elvis Teixeira
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
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <slope/figure_p.h>
#include <slope/view.h>

typedef struct _SlopeViewPrivate
{
  SlopeFigure *figure;
  gboolean     mouse_pressed;
} SlopeViewPrivate;

#define SLOPE_VIEW_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_VIEW_TYPE, SlopeViewPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(SlopeView, slope_view, GTK_TYPE_DRAWING_AREA)

static void _view_finalize(GObject *self);
static void _view_set_figure(SlopeView *self, SlopeFigure *figure);
static gboolean _view_on_draw(GtkWidget *self, cairo_t *cr, gpointer data);
static gboolean _view_on_mouse_event(GtkWidget *self,
                                     GdkEvent * gdk_event,
                                     gpointer   data);

static void slope_view_class_init(SlopeViewClass *klass)
{
  GObjectClass *object_klass = G_OBJECT_CLASS(klass);
  object_klass->finalize     = _view_finalize;
  klass->set_figure          = _view_set_figure;
}

static void slope_view_init(SlopeView *self)
{
  GtkWidget *       gtk_widget = GTK_WIDGET(self);
  SlopeViewPrivate *priv       = SLOPE_VIEW_GET_PRIVATE(self);
  priv->figure                 = NULL;
  priv->mouse_pressed          = FALSE;
  /* minimum width and height of the widget */
  gtk_widget_set_size_request(gtk_widget, 250, 250);
  /* select the types of events we want to be notified about */
  gtk_widget_add_events(gtk_widget,
                        GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK |
                            GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);
  /* set drawing callback */
  g_signal_connect(G_OBJECT(self), "draw", G_CALLBACK(_view_on_draw), NULL);
  /* set mouse event callbacks */
  g_signal_connect(G_OBJECT(self),
                   "button-press-event",
                   G_CALLBACK(_view_on_mouse_event),
                   GINT_TO_POINTER(SLOPE_MOUSE_PRESS));
  g_signal_connect(G_OBJECT(self),
                   "motion-notify-event",
                   G_CALLBACK(_view_on_mouse_event),
                   GINT_TO_POINTER(SLOPE_MOUSE_MOVE));
  g_signal_connect(G_OBJECT(self),
                   "button-release-event",
                   G_CALLBACK(_view_on_mouse_event),
                   GINT_TO_POINTER(SLOPE_MOUSE_RELEASE));
}

static void _view_finalize(GObject *self)
{
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
  if (priv->figure != NULL)
    {
      if (slope_figure_get_is_managed(priv->figure))
        {
          g_object_unref(priv->figure);
        }
      priv->figure = NULL;
    }
  G_OBJECT_CLASS(slope_view_parent_class)->finalize(self);
}

GtkWidget *slope_view_new()
{
  GtkWidget *self = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));
  return self;
}

GtkWidget *slope_view_new_with_figure(SlopeFigure *figure)
{
  GtkWidget *self = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));
  slope_view_set_figure(SLOPE_VIEW(self), figure);
  return self;
}

void slope_view_write_to_png(SlopeView * self,
                             const char *filename,
                             int         width,
                             int         height)
{
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
  if (priv->figure != NULL)
    {
      slope_figure_write_to_png(priv->figure, filename, width, height);
    }
}

static gboolean _view_on_draw(GtkWidget *self, cairo_t *cr, gpointer data)
{
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
  GtkAllocation     allocation;
  SlopeRect         rect;
  SLOPE_UNUSED(data);
  if (priv->figure == NULL)
    {
      return TRUE;
    }
  gtk_widget_get_allocation(self, &allocation);
  rect.x      = 0.0;
  rect.y      = 0.0;
  rect.width  = allocation.width;
  rect.height = allocation.height;
  slope_figure_draw(priv->figure, &rect, cr);
  return TRUE;
}

static gboolean _view_on_mouse_event(GtkWidget *self,
                                     GdkEvent * gdk_event,
                                     gpointer   data)
{
  SlopeViewPrivate *  priv       = SLOPE_VIEW_GET_PRIVATE(self);
  SlopeMouseEventType event_type = GPOINTER_TO_INT(data);
  SlopeMouseEvent     mouse_event;
  SLOPE_UNUSED(data);
  /* In case of move events we want to know if a mouse button is pressed */
  if (event_type == SLOPE_MOUSE_PRESS)
    {
      priv->mouse_pressed = TRUE;
      if (gdk_event->type == GDK_2BUTTON_PRESS)
        event_type = SLOPE_MOUSE_DOUBLE_PRESS;
    }
  else if (event_type == SLOPE_MOUSE_RELEASE)
    {
      priv->mouse_pressed = FALSE;
    }
  else if (event_type == SLOPE_MOUSE_MOVE)
    {
      if (priv->mouse_pressed == TRUE) event_type = SLOPE_MOUSE_MOVE_PRESSED;
    }
  mouse_event.type = event_type;
  /* check which mouse button was pressed, we have interest only in left
     in right buttons */
  if (gdk_event->button.button == 1)
    {
      mouse_event.button = SLOPE_MOUSE_BUTTON_LEFT;
    }
  else if (gdk_event->button.button == 3)
    {
      mouse_event.button = SLOPE_MOUSE_BUTTON_RIGHT;
    }
  else
    {
      mouse_event.button = SLOPE_MOUSE_BUTTON_NONE;
    }
  /* mouse pointer position in widget coordinates */
  mouse_event.x = gdk_event->button.x;
  mouse_event.y = gdk_event->button.y;
  /* finally send the event down to be handled by the figure, it's
     scales and elements */
  _figure_handle_mouse_event(priv->figure, &mouse_event);
  return FALSE;
}

static void _view_set_figure(SlopeView *self, SlopeFigure *figure)
{
  SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
  if (priv->figure != figure)
    {
      if (priv->figure != NULL)
        {
          _figure_set_view(priv->figure, NULL);
        }
      priv->figure = figure;
      if (priv->figure != NULL)
        {
          _figure_set_view(priv->figure, self);
        }
    }
}

SlopeFigure *slope_view_get_figure(SlopeFigure *self)
{
  if (self != NULL) return SLOPE_VIEW_GET_PRIVATE(self)->figure;
  return NULL;
}

void slope_view_redraw(SlopeView *self)
{
  gtk_widget_queue_draw(GTK_WIDGET(self));
}

void slope_view_set_figure(SlopeView *self, SlopeFigure *figure)
{
  SLOPE_VIEW_GET_CLASS(self)->set_figure(self, figure);
}

/* slope/view.c */
