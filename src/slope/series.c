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
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <slope/series_p.h>
#include <stdlib.h>


static void _slope_series_get_data_rect (const slope_item_t *self, slope_rect_t *rect);
static void _slope_series_get_figure_rect (const slope_item_t *self, slope_rect_t *rect);
static void _slope_series_check_ranges (slope_series_t *self);
static void _slope_series_draw (slope_item_t *self, cairo_t *cr);
static void _slope_series_draw_line (slope_item_t *self, cairo_t *cr);
static void _slope_series_draw_area_under (slope_item_t *self, cairo_t *cr);


static slope_item_class_t* _slope_series_get_class()
{
    static slope_item_class_t item_class;
    static slope_bool_t first_call = SLOPE_TRUE;
    
    if (first_call) {
        item_class.init = slope_series_init;
        item_class.finalize = slope_series_finalize;
        item_class.draw = _slope_series_draw;
        item_class.get_data_rect = _slope_series_get_data_rect;
        item_class.get_figure_rect = _slope_series_get_figure_rect;
        first_call = SLOPE_FALSE;
    }
    return &item_class;
}


slope_item_t* slope_series_new_for_data (const double *vx, const double *vy,
                                         int size, const char *name,
                                         slope_color_t stroke_color, slope_color_t fill_color)
{
    slope_series_t *self = SLOPE_ALLOC(slope_series_t);
    slope_series_private_t *priv = SLOPE_ALLOC(slope_series_private_t);
    
    SLOPE_ITEM(self)->_private = SLOPE_ITEM_PRIVATE(priv);
    SLOPE_ITEM(self)->_class = _slope_series_get_class();
    SLOPE_ITEM_GET_CLASS(self)->init(SLOPE_ITEM(self));
    
    priv->stroke_color = stroke_color;
    priv->fill_color = fill_color;
    priv->vx = vx;
    priv->vy = vy;
    priv->size = size;
    priv->style = SLOPE_SERIES_LINE;
    slope_item_set_name(SLOPE_ITEM(self), name);
    _slope_series_check_ranges(self);
    
    return SLOPE_ITEM(self);
}


void slope_series_init (slope_item_t *self)
{
    slope_series_private_t *priv = SLOPE_SERIES_GET_PRIVATE(self);
    slope_item_init(self);
    
    priv->xmin = 0.0;
    priv->xmax = 1.0;
    priv->ymin = 0.0;
    priv->ymax = 1.0;
}


void slope_series_finalize (slope_item_t *self)
{
    slope_item_finalize(self);
}


static void _slope_series_draw (slope_item_t *self, cairo_t *cr)
{
    slope_series_private_t *priv = SLOPE_SERIES_GET_PRIVATE(self);

    if (priv->size == 0)
        return;

    switch (priv->style) {
    case SLOPE_SERIES_LINE:
        _slope_series_draw_line(self, cr);
        break;
    case SLOPE_SERIES_AREAUNDER:
        _slope_series_draw_area_under(self, cr);
        break;
    }
}


static void _slope_series_draw_line (slope_item_t *self, cairo_t *cr)
{
    slope_series_private_t *priv = SLOPE_SERIES_GET_PRIVATE(self);
    slope_item_private_t *item_priv = SLOPE_ITEM_GET_PRIVATE(self);
    slope_point_t data_point, figure_point;
    double x1, y1;
    double x2, y2;
    int k;
    
    data_point.x = priv->vx[0];
    data_point.y = priv->vy[0];
    slope_scale_map (item_priv->scale, &figure_point, &data_point);
    
    x1 = figure_point.x;
    y1 = figure_point.y;
    cairo_move_to(cr, x1, y1);
    
    for (k=1; k<priv->size; ++k) {
        data_point.x = priv->vx[k];
        data_point.y = priv->vy[k];
        slope_scale_map (item_priv->scale, &figure_point, &data_point);
        
        x2 = figure_point.x;
        y2 = figure_point.y;
        cairo_line_to(cr, x2, y2);
    }
    slope_cairo_set_color(cr, priv->stroke_color);
    cairo_stroke(cr);
}


static void _slope_series_draw_area_under (slope_item_t *self, cairo_t *cr)
{
    slope_series_private_t *priv = SLOPE_SERIES_GET_PRIVATE(self);
    slope_item_private_t *item_priv = SLOPE_ITEM_GET_PRIVATE(self);
    slope_point_t data_point, figure_point;
    cairo_path_t *path;
    double x1, y1;
    double x2, y2;
    double x0, y0;
    int k;
    
    data_point.x = priv->vx[0];
    data_point.y = priv->vy[0];
    slope_scale_map (item_priv->scale, &figure_point, &data_point);
    
    x1 = figure_point.x;
    y1 = figure_point.y;
    x0 = figure_point.x;
    cairo_move_to(cr, x1, y1);
    
    for (k=1; k<priv->size; ++k) {
        data_point.x = priv->vx[k];
        data_point.y = priv->vy[k];
        slope_scale_map (item_priv->scale, &figure_point, &data_point);
        
        x2 = figure_point.x;
        y2 = figure_point.y;
        cairo_line_to(cr, x2, y2);
    }
    
    path = cairo_copy_path(cr);
    
    data_point.x = priv->vx[k-1];
    data_point.y = 0.0;
    slope_scale_map (item_priv->scale, &figure_point, &data_point);
    
    x1 = figure_point.x;
    y0 = figure_point.y;
    cairo_line_to(cr, x1, y0);
    
    cairo_line_to(cr, x0, y0);
    cairo_close_path(cr);
    
    slope_cairo_set_color(cr, priv->fill_color);
    cairo_fill(cr);
    slope_cairo_set_color(cr, priv->stroke_color);
    cairo_append_path(cr, path);
    cairo_stroke(cr);
    cairo_path_destroy(path);
}


static void _slope_series_get_data_rect (const slope_item_t *self, slope_rect_t *rect)
{
    slope_series_private_t *priv = SLOPE_SERIES_GET_PRIVATE(self);
    rect->x = priv->xmin;
    rect->y = priv->ymin;
    rect->width = priv->xmax - priv->xmin;
    rect->height = priv->ymax - priv->ymin;
}


static void _slope_series_get_figure_rect (const slope_item_t *self, slope_rect_t *rect)
{
    /* TODO */
}


static void _slope_series_check_ranges (slope_series_t *self)
{
    slope_series_private_t *priv = SLOPE_SERIES_GET_PRIVATE(self);
    const double *vx = priv->vx;
    const double *vy = priv->vy;
    int k;
    
    if (priv->size == 0) {
        priv->xmin = priv->xmax = 0.0;
        priv->ymin = priv->ymax = 0.0;
        return;
    }

    priv->xmin = priv->xmax = vx[0];
    priv->ymin = priv->ymax = vy[0];
    
    for (k=1; k<priv->size; ++k) {
        if (vx[k] < priv->xmin) priv->xmin = vx[k];
        if (vx[k] > priv->xmax) priv->xmax = vx[k];
        if (vy[k] < priv->ymin) priv->ymin = vy[k];
        if (vy[k] > priv->ymax) priv->ymax = vy[k];
    }
}


void slope_series_set_style (slope_item_t *self, int style)
{
    SLOPE_SERIES_GET_PRIVATE(self)->style = (slope_series_style_t) style;
}

/* slope/series.c */
