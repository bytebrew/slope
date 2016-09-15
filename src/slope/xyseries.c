/*
 * Copyright (C) 2016  Elvis Teixeira
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

#include <slope/xyseries.h>


typedef struct
_SlopeXySeriesPrivate
{
    double x_min, x_max;
    double y_min, y_max;

    const double *x_vec, *y_vec;
    long n_pts;
}
SlopeXySeriesPrivate;


void _xyseries_draw (SlopeItem *self, cairo_t *cr);
void _xyseries_finalize (GObject *self);
void _xyseries_get_figure_rect (SlopeItem *self, SlopeRect *rect);
void _xyseries_get_data_rect (SlopeItem *self, SlopeRect *rect);



#define SLOPE_XYSERIES_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_XYSERIES_TYPE, SlopeXySeriesPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeXySeries, slope_xyseries, SLOPE_ITEM_TYPE)


static void
slope_xyseries_class_init (SlopeXySeriesClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);
    SlopeItemClass *item_klass = SLOPE_ITEM_CLASS(klass);

    object_klass->finalize = _xyseries_finalize;
    item_klass->draw = _xyseries_draw;
    item_klass->get_data_rect = _xyseries_get_data_rect;
    item_klass->get_figure_rect = _xyseries_get_figure_rect;
}


static void
slope_xyseries_init (SlopeXySeries *self)
{
    SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);

    priv->n_pts = 0L;
}


void _xyseries_finalize (GObject *self)
{

}


void _xyseries_draw (SlopeItem *self, cairo_t *cr)
{

}


void _xyseries_get_figure_rect (SlopeItem *self, SlopeRect *rect)
{

}


void _xyseries_get_data_rect (SlopeItem *self, SlopeRect *rect)
{

}

/* slope/xyseries.c */
