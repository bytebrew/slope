/*
 * Copyright (C) 2019  Elvis Teixeira
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

#include "slope/figure_p.h"
#include "slope/view_p.h"
#include "slope/item_p.h"
#include "slope/color.h"
#include "slope/drawing.h"


G_DEFINE_TYPE_WITH_PRIVATE (SlopeView, slope_view, GTK_TYPE_DRAWING_AREA)


static gboolean view_on_draw (GtkWidget *widget, cairo_t *cr, gpointer data);
static void view_dispose (GObject *gobject);


static void
slope_view_class_init (SlopeViewClass *klass) {
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->dispose = view_dispose;
}


static void
slope_view_init (SlopeView *self) {
  SlopeViewPrivate *priv = slope_view_get_instance_private (self);

  priv->figure = NULL;

  g_signal_connect (G_OBJECT (self), "draw", G_CALLBACK (view_on_draw), NULL);
}


static void
view_dispose (GObject *gobject) {
  SlopeViewPrivate *priv = slope_view_get_instance_private (SLOPE_VIEW (gobject));

  if (priv->figure) {
      g_object_unref (priv->figure);
      priv->figure = NULL;
  }

  G_OBJECT_CLASS (slope_view_parent_class)->dispose (gobject);
}


GtkWidget*
slope_view_new (void) {
    return GTK_WIDGET (g_object_new(SLOPE_TYPE_VIEW, NULL));
}


SlopeFigure*
slope_view_get_figure (SlopeView *self) {
    SlopeViewPrivate *priv = slope_view_get_instance_private (self);
    return priv->figure;
}


void
slope_view_set_figure (SlopeView *self, SlopeFigure *figure, gboolean own) {
    SlopeViewPrivate *priv = slope_view_get_instance_private (self);
    SlopeFigurePrivate *fig_priv;

    g_return_if_fail(SLOPE_IS_FIGURE (figure));

    priv->figure = own ? figure : g_object_ref (figure);
    fig_priv = figure_get_private(priv->figure);

    fig_priv->view = self;
}


static gboolean
view_on_draw (GtkWidget *widget, cairo_t *cr, gpointer data) {
    SlopeViewPrivate *priv = slope_view_get_instance_private (SLOPE_VIEW (widget));
    SlopeRect rect;
    (void) data;

    if (!priv->figure) {
        GdkRGBA color;
        GtkStyleContext *context;

        context = gtk_widget_get_style_context (widget);

        gtk_render_background (
                context,
                cr,
                0, 0,
                gtk_widget_get_allocated_width (widget),
                gtk_widget_get_allocated_height (widget));

        gtk_style_context_get_color (
                context,
                gtk_style_context_get_state (context),
                &color);

        gdk_cairo_set_source_rgba (cr, &color);
        cairo_fill (cr);
       return FALSE;
    }

    rect.x = 0.0;
    rect.y = 0.0;
    rect.width = (double) gtk_widget_get_allocated_width (widget);
    rect.height = (double) gtk_widget_get_allocated_height (widget);

    if (rect.width < 2.0 || rect.height < 2.0) {
        return FALSE;
    }

    slope_figure_draw (priv->figure, cr, &rect);
    return FALSE;
}


SlopeViewPrivate*
view_get_private (SlopeView *self) {
    return slope_view_get_instance_private (SLOPE_VIEW (self));
}
