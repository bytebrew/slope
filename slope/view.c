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

#include "slope/view.h"
#include "slope/figure.h"
#include "slope/color.h"
#include "slope/drawing.h"


typedef struct {
  SlopeFigure *figure;
} SlopeViewPrivate;


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

  g_signal_connect (G_OBJECT (self), "draw", G_CALLBACK (view_on_draw), NULL);

  priv->figure = slope_figure_new();
}


static void
view_dispose (GObject *gobject) {
  SlopeViewPrivate *priv = slope_view_get_instance_private (SLOPE_VIEW (gobject));
  if (!priv->figure)
      return;

  g_object_unref (G_OBJECT (priv->figure));
  priv->figure = NULL;

  G_OBJECT_CLASS (slope_view_parent_class)->dispose (gobject);
}


GtkWidget *slope_view_new (void) {
    return GTK_WIDGET (g_object_new(SLOPE_TYPE_VIEW, NULL));
}


SlopeFigure *slope_view_get_figure (SlopeView *self) {
    SlopeViewPrivate *priv = slope_view_get_instance_private (self);
    return priv->figure;
}


static gboolean view_on_draw (GtkWidget *widget, cairo_t *cr, gpointer data) {
    SlopeViewPrivate *priv = slope_view_get_instance_private (SLOPE_VIEW (widget));
    SlopeRect rect;
    (void) data;

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
