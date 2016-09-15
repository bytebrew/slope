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
#include <slope/scale.h>


typedef struct
_SlopeXySeriesPrivate
{
    double x_min, x_max;
    double y_min, y_max;

    const double *x_vec, *y_vec;
    long n_pts;

    SlopeColor stroke_color;
    SlopeColor fill_color;
    double line_width;
    double symbol_radius;
    int mode;
}
SlopeXySeriesPrivate;


static void _xyseries_draw (SlopeItem *self, cairo_t *cr);
static void _xyseries_finalize (GObject *self);
static void _xyseries_get_figure_rect (SlopeItem *self, SlopeRect *rect);
static void _xyseries_get_data_rect (SlopeItem *self, SlopeRect *rect);
static void _xyseries_check_ranges (SlopeXySeries *self);

static void _xyseries_draw_line (SlopeXySeries *self, cairo_t *cr);
static void _xyseries_draw_circles (SlopeXySeries *self, cairo_t *cr);


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
    priv->mode = SLOPE_SERIES_CIRCLES;
    priv->stroke_color = SLOPE_BLUE;
    priv->fill_color = SLOPE_RED;
    priv->line_width = 1.32;
    priv->symbol_radius = 2.5;
}


void _xyseries_finalize (GObject *self)
{

}


SlopeItem *slope_xyseries_new(void)
{
    SlopeItem *self = SLOPE_ITEM(g_object_new(SLOPE_XYSERIES_TYPE, NULL));

    return self;
}


void slope_xyseries_set_data (SlopeXySeries *self, const double *x_vec,
                              const double *y_vec, long n_pts)
{
    SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);

    priv->x_vec = x_vec;
    priv->y_vec = y_vec;
    priv->n_pts = n_pts;

    _xyseries_check_ranges(self);
}


static
void _xyseries_draw (SlopeItem *self, cairo_t *cr)
{
    SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);

    if (priv->mode == SLOPE_SERIES_LINE) {
        _xyseries_draw_line(SLOPE_XYSERIES(self), cr);
    } else if (priv->mode == SLOPE_SERIES_CIRCLES) {
        _xyseries_draw_circles(SLOPE_XYSERIES(self), cr);
    }
}


static
void _xyseries_draw_line (SlopeXySeries *self, cairo_t *cr)
{
    SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);
    SlopeScale *scale = slope_item_get_scale(SLOPE_ITEM(self));
    SlopePoint p1, p2, p;
    double dx, dy, d2;
    long k;

    p.x = priv->x_vec[0];
    p.y = priv->y_vec[0];
    slope_scale_map(scale, &p1, &p);
    cairo_new_path(cr);
    cairo_move_to(cr, p1.x, p1.y);

    for (k=1L; k<priv->n_pts; ++k) {
        p.x = priv->x_vec[k];
        p.y = priv->y_vec[k];
        slope_scale_map(scale, &p2, &p);

        dx = p2.x - p1.x;
        dy = p2.y - p1.y;
        d2 = dx*dx + dy*dy;

        if (d2 >= 9.0) {
            cairo_line_to(cr, p2.x, p2.y);
            p1 = p2;
        }
    }

    cairo_set_line_width(cr, priv->line_width);
    slope_cairo_set_color(cr, priv->stroke_color);
    cairo_stroke(cr);
}


static
void _xyseries_draw_circles (SlopeXySeries *self, cairo_t *cr)
{
    SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);
    SlopeScale *scale = slope_item_get_scale(SLOPE_ITEM(self));
    SlopePoint dat_p, fig_p;
    long k;

    for (k=0L; k<priv->n_pts; ++k) {
        fig_p.x = priv->x_vec[k];
        fig_p.y = priv->y_vec[k];
        slope_scale_map(scale, &dat_p, &fig_p);

        slope_cairo_circle(cr, &dat_p, priv->symbol_radius);

        if (!SLOPE_COLOR_IS_NULL(priv->fill_color) &&
            !SLOPE_COLOR_IS_NULL(priv->stroke_color))
        {
            slope_cairo_set_color(cr, priv->fill_color);
            cairo_fill_preserve(cr);
            slope_cairo_set_color(cr, priv->stroke_color);
            cairo_stroke(cr);
        }
        else if(!SLOPE_COLOR_IS_NULL(priv->fill_color))
        {
            slope_cairo_set_color(cr, priv->fill_color);
            cairo_fill(cr);
        }
        else if(!SLOPE_COLOR_IS_NULL(priv->stroke_color))
        {
            slope_cairo_set_color(cr, priv->stroke_color);
            cairo_stroke(cr);
        }
    }
}


static
void _xyseries_get_figure_rect (SlopeItem *self, SlopeRect *rect)
{
    // TODO
}


static
void _xyseries_get_data_rect (SlopeItem *self, SlopeRect *rect)
{
    SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);

    rect->x = priv->x_min;
    rect->y = priv->y_min;
    rect->width = priv->x_max - priv->x_min;
    rect->height = priv->y_max - priv->y_min;
}


static
void _xyseries_check_ranges (SlopeXySeries *self)
{
    SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);
    const double *x = priv->x_vec;
    const double *y = priv->y_vec;
    long k;

    priv->x_min = priv->x_max = x[0];
    priv->y_min = priv->y_max = x[0];

    for (k=1L; k<priv->n_pts; ++k) {
        if (x[k] < priv->x_min) priv->x_min = x[k];
        if (x[k] > priv->x_max) priv->x_max = x[k];
        if (y[k] < priv->y_min) priv->y_min = y[k];
        if (y[k] > priv->y_max) priv->y_max = y[k];
    }
}

/* slope/xyseries.c */
