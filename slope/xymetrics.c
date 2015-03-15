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

#include "slope/legend_p.h"
#include "slope/xymetrics_p.h"
#include "slope/xyframe_p.h"
#include "slope/xydata_p.h"
#include <stdlib.h>


slope_metrics_t* slope_xymetrics_create()
{
    slope_xymetrics_t *self = malloc(sizeof(slope_xymetrics_t));
    slope_metrics_t *parent = (slope_metrics_t*) self;
    parent->data = NULL;
    parent->visible = 1;
    parent->legend = _slope_legend_create(parent);
    parent->_cleanup_fn = _slope_xymetrics_cleanup;
    parent->_update_fn = _slope_xymetrics_update;
    parent->_draw_fn = _slope_xymetrics_draw;
    self->x_low_bound = self->x_up_bound = 80.0;
    self->y_low_bound = self->y_up_bound = 50.0;
    self->frame = _slope_xyframe_create(parent);
    return parent;
}


void _slope_xymetrics_cleanup (slope_metrics_t *parent)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) parent;
    _slope_frame_destroy(self->frame);
}


slope_frame_t* slope_xymetrics_get_frame (slope_metrics_t *metrics)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    return self->frame;
}


double slope_xymetrics_map_x (slope_metrics_t *metrics, double x)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    double ret = (x - self->xmin) /self->width;
    return self->xmin_scene + ret*self->width_scene;
}


double slope_xymetrics_map_y (slope_metrics_t *metrics, double y)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    double ret = (y - self->ymin) /self->height;
    return self->ymax_scene - ret*self->height_scene;
}


void _slope_xymetrics_update (slope_metrics_t *metrics)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    if (metrics->data == NULL) {
        self->xmin = 0.0;
        self->xmax = 1.0;
        self->ymin = 0.0;
        self->ymax = 1.0;
        return;
    }
    slope_iterator_t *iter = slope_list_first(metrics->data);
    slope_xydata_t *dat = (slope_xydata_t*) slope_iterator_data(iter);
    self->xmin = dat->xmin;
    self->xmax = dat->xmax;
    self->ymin = dat->ymin;
    self->ymax = dat->ymax;

    slope_iterator_next(&iter);
    while (iter) {
        dat = (slope_xydata_t*) slope_iterator_data(iter);
        if (dat->xmin < self->xmin) self->xmin = dat->xmin;
        if (dat->xmax > self->xmax) self->xmax = dat->xmax;
        if (dat->ymin < self->ymin) self->ymin = dat->ymin;
        if (dat->ymax > self->ymax) self->ymax = dat->ymax;
        slope_iterator_next(&iter);
    }
    self->width = self->xmax - self->xmin;
    self->height = self->ymax - self->ymin;
}


void _slope_xymetrics_draw (slope_metrics_t *metrics, cairo_t *cr,
                            slope_rect_t *area)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;

    /* get scene geometry */
    self->xmin_scene = area->x + self->x_low_bound;
    self->ymin_scene = area->y + self->y_low_bound;
    self->xmax_scene = area->x + area->width - self->x_up_bound;
    self->ymax_scene = area->y + area->height - self->y_up_bound;
    self->width_scene = self->xmax_scene - self->xmin_scene;
    self->height_scene = self->ymax_scene - self->ymin_scene;

    /* draw contents */
    cairo_save(cr);
    cairo_stroke(cr);
    cairo_rectangle(
        cr, self->xmin_scene, self->ymin_scene,
        self->width_scene, self->height_scene);
    cairo_clip(cr);
    slope_iterator_t *iter = slope_list_first(metrics->data);
    while (iter) {
        slope_data_t *data =
            (slope_data_t*) slope_iterator_data(iter);
        _slope_data_draw(data, cr, metrics);
        slope_iterator_next(&iter);
    }
    cairo_restore(cr);
    /* draw frame */
    _slope_xyframe_draw(self->frame,cr);
    
    slope_point_t legend_pos;
    legend_pos.x = self->xmin_scene + 10.0;
    legend_pos.y = self->ymin_scene + 10.0;
    _slope_legend_draw(metrics->legend, cr, &legend_pos);
}

/* slope/xymetrics.c */
