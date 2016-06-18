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

#include "slope/widget.h"


#define SLOPE_WIDGET_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_WIDGET_TYPE, SlopeWidgetPrivate))


static gboolean _slope_widget_draw_callback (GtkWidget *widget, cairo_t *cr, gpointer *data);
static gboolean _slope_widget_mouse_press_callback (GtkWidget *widget, GdkEvent *event, gpointer *data);
static gboolean _slope_widget_mouse_move_callback (GtkWidget *widget, GdkEvent *event, gpointer *data);
static gboolean _slope_widget_mouse_release_callback (GtkWidget *widget, GdkEvent *event, gpointer *data);


typedef struct
_SlopeWidgetPrivate
{
    slope_figure_t *figure;
    slope_bool_t own_figure;
    slope_point_t mouse_p1;
    slope_point_t mouse_p2;
}
SlopeWidgetPrivate;


G_DEFINE_TYPE(SlopeWidget, slope_widget, GTK_TYPE_DRAWING_AREA);


static void
slope_widget_finalize (GObject *gobject)
{
    SlopeWidgetPrivate *m = SLOPE_WIDGET_GET_PRIVATE(gobject);
    GObjectClass *parent_class = g_type_class_peek_parent(G_OBJECT_GET_CLASS(gobject));

    if (m->figure && m->own_figure) {
        slope_figure_destroy(m->figure);
    }

    G_OBJECT_CLASS(parent_class)->finalize(gobject);
}


static void
slope_widget_class_init (SlopeWidgetClass *klass)
{
    GtkWidgetClass *widget_klass = GTK_WIDGET_CLASS (klass);
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->finalize = slope_widget_finalize;
    
    g_type_class_add_private(klass, sizeof(SlopeWidgetPrivate));
}


static void
slope_widget_init (SlopeWidget *widget)
{
    SlopeWidgetPrivate *priv = SLOPE_WIDGET_GET_PRIVATE (widget);
    GtkWidget *gtk_widget = GTK_WIDGET (widget);
    
    gtk_widget_add_events(gtk_widget,
                          GDK_EXPOSURE_MASK
                          |GDK_BUTTON_MOTION_MASK
                          |GDK_BUTTON_PRESS_MASK
                          |GDK_BUTTON_RELEASE_MASK);
    
    g_signal_connect(G_OBJECT(gtk_widget), "draw",
                     G_CALLBACK(_slope_widget_draw_callback), NULL);
    g_signal_connect(G_OBJECT(gtk_widget), "button-press-event",
                     G_CALLBACK(_slope_widget_mouse_press_callback), NULL);
}


GtkWidget *
slope_widget_new ()
{
    GtkWidget *widget = GTK_WIDGET(g_object_new(SLOPE_WIDGET_TYPE, NULL));
    SlopeWidgetPrivate *priv = SLOPE_WIDGET_GET_PRIVATE (widget);
    priv->figure = slope_figure_new("figure");
    priv->own_figure = SLOPE_TRUE;
    return widget;
}


GtkWidget *
slope_widget_new_for_figure (slope_figure_t *figure)
{
    GtkWidget *widget = GTK_WIDGET(g_object_new(SLOPE_WIDGET_TYPE, NULL));
    SlopeWidgetPrivate *priv = SLOPE_WIDGET_GET_PRIVATE (widget);
    priv->figure = figure;
    priv->own_figure = SLOPE_FALSE;
    return widget;
}


slope_figure_t*
slope_widget_get_figure (GtkWidget *widget)
{
    SlopeWidgetPrivate *priv = SLOPE_WIDGET_GET_PRIVATE (widget);
    
    if (priv != NULL) {
        return priv->figure;
    }
    return NULL;
}


static gboolean
_slope_widget_draw_callback (GtkWidget *widget, cairo_t *cr, gpointer *data)
{
    SlopeWidgetPrivate *priv = SLOPE_WIDGET_GET_PRIVATE (widget);
    GtkAllocation allocation;
    slope_rect_t rect;
    
    gtk_widget_get_allocation(widget, &allocation);
    slope_rect_set(&rect, 0.0, 0.0, allocation.width, allocation.height);

    slope_figure_draw(priv->figure, &rect, cr);
    return TRUE;
}


static gboolean
_slope_widget_mouse_press_callback (GtkWidget *widget, GdkEvent *event, gpointer *data)
{

    return TRUE;
}


static gboolean
_slope_widget_mouse_move_callback (GtkWidget *widget, GdkEvent *event, gpointer *data)
{

    return TRUE;
}


static gboolean
_slope_widget_mouse_release_callback (GtkWidget *widget, GdkEvent *event, gpointer *data)
{

    return TRUE;
}

/* slope/widget.c */
