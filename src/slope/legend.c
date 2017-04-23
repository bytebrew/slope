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

#include <slope/legend.h>
#include <slope/scale.h>

typedef struct
_SlopeLegendPrivate
{
    guint32 orientation;

    double x;
    double y;

    double stroke_width;
    SlopeColor stroke_color;
    SlopeColor fill_color;

    SlopePoint p1, p2;
}
SlopeLegendPrivate;


#define SLOPE_LEGEND_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_LEGEND_TYPE, SlopeLegendPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeLegend, slope_legend, SLOPE_ITEM_TYPE)


static void _legend_finalize (GObject *self);
static void _legend_draw (SlopeItem *self, cairo_t *cr);
static void _legend_get_figure_rect (SlopeItem *self, SlopeRect *rect);
static void _legend_get_data_rect (SlopeItem *self, SlopeRect *rect);


static void
slope_legend_class_init (SlopeLegendClass *klass) {
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);
    SlopeItemClass *item_klass = SLOPE_ITEM_CLASS(klass);

    object_klass->finalize = _legend_finalize;

    item_klass->draw = _legend_draw;
    item_klass->get_data_rect = _legend_get_data_rect;
    item_klass->get_figure_rect = _legend_get_figure_rect;
}


static void
slope_legend_init (SlopeLegend *self) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);

    priv->orientation = SLOPE_VERTICAL;
    priv->fill_color = SLOPE_WHITE;
    priv->stroke_color = SLOPE_BLACK;
    priv->stroke_width = 1.0;
}


static void _legend_finalize (GObject *self) {

    G_OBJECT_CLASS(slope_legend_parent_class)->finalize(self);
}


static void _legend_draw (SlopeItem *self, cairo_t *cr) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    SlopeScale *scale = slope_item_get_scale(self);

    /* evaluate the rectangle occupied by the legend
       top left point id easy because it is given by the user
       as the x and y coordinates in set_position.
       The second is tricky, we have to evaluate the legend
       entries extents */
    SlopePoint data_p1;
    data_p1.x = priv->x;
    data_p1.y = priv->y;
    slope_scale_map(scale, &priv->p1, &data_p1);

}


static void _legend_get_figure_rect (SlopeItem *self, SlopeRect *rect) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    rect->x = priv->p1.x;
    rect->y = priv->p1.y;
    rect->width = priv->p2.x - priv->p1.x;
    rect->height = priv->p2.y - priv->p1.y;
}


static void _legend_get_data_rect (SlopeItem *self, SlopeRect *rect) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    SlopeScale *scale = slope_item_get_scale(self);
    SlopePoint data_p2;
    slope_scale_unmap(scale, &data_p2, &priv->p2);
    rect->x = priv->x;
    rect->y = data_p2.y;
    rect->width = data_p2.x - priv->x;
    rect->height = priv->y - data_p2.y;
}

void slope_legend_set_orientation (SlopeLegend *self,
                                   SlopeOrientation orientation) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    priv->orientation = orientation;
}

void slope_legend_set_position (SlopeLegend *self, double x, double y) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    priv->x = x;
    priv->y = y;
}

/* slope/legend.c */
