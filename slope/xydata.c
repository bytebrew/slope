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

#include "slope/xydata_p.h"
#include <stdlib.h>
#include <string.h>


slope_data_t*
slope_xydata_create_simple (double *vx, double *vy,
                            const int n,
                            const char *name,
                            slope_color_name_t color,
                            slope_scatter_t scatter)
{
    slope_xydata_t *self = malloc(sizeof(slope_xydata_t));
    slope_data_t *parent = (slope_data_t*) self;
    
    slope_xydata_set(parent, vx, vy, n);
    slope_color_set_name(&self->color, color);
    self->scatter = SLOPE_LINE;
    parent->visible = 1;
    parent->name = strdup(name);
    parent->_cleanup_fn = NULL;
    parent->_draw_fn = _slope_xydata_draw;
    return parent;
}


void _slope_xydata_draw (slope_data_t *data, cairo_t *cr,
                         slope_metrics_t *metrics)
{
    slope_xydata_t *self = (slope_xydata_t*) data;
    
    cairo_set_source_rgba(
        cr, self->color.red, self->color.green,
        self->color.blue, self->color.alpha);
    
    switch (self->scatter) {
        case SLOPE_LINE:
            _slope_xydata_draw_line(data, cr, metrics);
            break;
        default:
            break;
    }
}


void _slope_xydata_draw_line (slope_data_t *data, cairo_t *cr,
                              slope_metrics_t *metrics)
{
    slope_xydata_t *self = (slope_xydata_t*) data;
    
    const double *vx = self->vx;
    const double *vy = self->vy;
    const int n = self->n;
    
    double x1 = slope_xymetrics_map_x(metrics, vx[0]);
    double y1 = slope_xymetrics_map_y(metrics, vy[0]);
    cairo_move_to(cr, x1, y1);
    
    int k;
    for (k=1; k<n; k++) {
        double x2 = slope_xymetrics_map_x(metrics, vx[k]);
        double y2 = slope_xymetrics_map_y(metrics, vy[k]);
        cairo_line_to(cr, x2, y2);
        x1 = x2;
        y1 = y2;
    }
    cairo_stroke(cr);
}


double slope_xydata_x_max (slope_data_t *data)
{
    if (data == NULL) {
        return -1.0;
    }
    slope_xydata_t *self = (slope_xydata_t*) data;
    return self->xmax;
}


double slope_xydata_x_min (slope_data_t *data)
{
    if (data == NULL) {
        return -1.0;
    }
    slope_xydata_t *self = (slope_xydata_t*) data;
    return self->xmin;
}


double slope_xydata_y_max (slope_data_t *data)
{
    if (data == NULL) {
        return -1.0;
    }
    slope_xydata_t *self = (slope_xydata_t*) data;
    return self->ymax;
}


double slope_xydata_y_min (slope_data_t *data)
{
    if (data == NULL) {
        return -1.0;
    }
    slope_xydata_t *self = (slope_xydata_t*) data;
    return self->ymin;
}


void slope_xydata_set (slope_data_t *data,
                        double *vx, double *vy,
                        const int n)
{
    slope_xydata_t *self = (slope_xydata_t*) data;
    self->vx = vx;
    self->vy = vy;
    self->n = n;
    _slope_xydata_check_ranges(data);
}


void _slope_xydata_check_ranges (slope_data_t *data)
{
    slope_xydata_t *self = (slope_xydata_t*) data;
    const double *vx = self->vx;
    const double *vy = self->vy;
    const int n = self->n;
    self->xmin = self->xmax = vx[0];
    self->ymin = self->ymax = vy[0];
    
    int k;
    for (k=1; k<n; k++) {
        if (vx[k] < self->xmin) self->xmin = vx[k];
        if (vx[k] > self->xmax) self->xmax = vx[k];
        if (vy[k] < self->ymin) self->ymin = vy[k];
        if (vy[k] > self->ymax) self->ymax = vy[k];
    }
}

/* slope/xydata.c */
