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


G_DEFINE_TYPE_WITH_PRIVATE (SlopeFigure, slope_figure, G_TYPE_OBJECT)


static void figure_draw (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect);
static void figure_draw_bg (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect);
static void figure_dispose (GObject *gobject);


static void
slope_figure_class_init (SlopeFigureClass *klass) {
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);
    klass->draw = figure_draw;
    object_klass->dispose = figure_dispose;
}


static void
slope_figure_init (SlopeFigure *self) {
  SlopeFigurePrivate *priv = slope_figure_get_instance_private (self);

  priv->back_color = SLOPE_WHITE;
  priv->view = NULL;
  SLOPE_LIST_INIT(&priv->root_items);
}


static void
figure_dispose (GObject *gobject) {
  G_OBJECT_CLASS (slope_figure_parent_class)->dispose (gobject);
}


SlopeFigure*
slope_figure_new(void) {
    return SLOPE_FIGURE (g_object_new(SLOPE_TYPE_FIGURE, NULL));
}


static void
figure_draw (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect) {

    /* Drawing context to be used by items */
    SlopeDC dc;
    dc.cr = cr;
    dc.parent_rect = *rect;

    figure_draw_bg (self, cr, rect);
}


static void
figure_draw_bg (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect) {
    SlopeFigurePrivate *priv = slope_figure_get_instance_private (SLOPE_FIGURE (self));

    if (slope_rgba_visible(priv->back_color)) {
        slope_set_color(cr, priv->back_color);
        slope_rect(cr, rect);
        cairo_fill(cr);
    }
}


void
slope_figure_draw (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect) {
    SLOPE_FIGURE_GET_CLASS(self)->draw(self, cr, rect);
}


SlopeFigurePrivate*
figure_get_private (SlopeFigure *self) {
    return slope_figure_get_instance_private (SLOPE_FIGURE (self));
}


void
slope_figure_add_item (SlopeFigure *self, SlopeItem *item, gboolean own) {
// TODO
}
