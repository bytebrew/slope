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
#include <cairo.h>

#define SYMBRAD 2.0
#define SYMBRADSQR 4.0
#define THREESYMBRADSQR 36.0


slope_data_t*
slope_xydata_create_simple (double *vx, double *vy,
                            const int n,
                            const char *name,
                            slope_color_name_t color,
                            slope_scatter_t scatter)
{
    slope_xydata_t *self = malloc(sizeof(slope_xydata_t));
    slope_data_t *parent = (slope_data_t*) self;

    parent->name = NULL;
    slope_xydata_set(parent, vx, vy, n,
                     name, color, scatter);
    self->antialias = SLOPE_TRUE;
    self->line_width = 1.5;
    self->fill_symbol = SLOPE_FALSE;
    parent->visible = SLOPE_TRUE;
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
    if (self->antialias) {
        cairo_set_antialias(
            cr, CAIRO_ANTIALIAS_SUBPIXEL);
    }
    else {
        cairo_set_antialias(
            cr, CAIRO_ANTIALIAS_NONE);
    }
    cairo_set_line_width(cr,self->line_width);

    switch (self->scatter) {
        case SLOPE_LINE:
            _slope_xydata_draw_line(data, cr, metrics);
            break;
        case SLOPE_CIRCLES:
            _slope_xydata_draw_circles(data, cr, metrics);
            break;
        case SLOPE_TRIANGLES:
            _slope_xydata_draw_triangles(data, cr, metrics);
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
        
        double dx = x2 - x1;
        double dy = y2 - y1;
        double distsqr = dx*dx + dy*dy;
        
        if (distsqr >= SYMBRADSQR) {
            cairo_line_to(cr, x2, y2);
            x1 = x2;
            y1 = y2;
        }
    }
    cairo_stroke(cr);
}


void _slope_xydata_draw_circles (slope_data_t *data, cairo_t *cr,
                                 slope_metrics_t *metrics)
{
    slope_xydata_t *self = (slope_xydata_t*) data;
    
    const double *vx = self->vx;
    const double *vy = self->vy;
    const int n = self->n;
    
    double x1 = slope_xymetrics_map_x(metrics, vx[0]);
    double y1 = slope_xymetrics_map_y(metrics, vy[0]);
    cairo_move_to(cr, x1+SYMBRAD, y1);
    cairo_arc(cr, x1, y1, SYMBRAD, 0.0, 6.283185);
    if (self->fill_symbol) cairo_fill(cr);
    
    int k;
    for (k=1; k<n; k++) {
        double x2 = slope_xymetrics_map_x(metrics, vx[k]);
        double y2 = slope_xymetrics_map_y(metrics, vy[k]);
        
        double dx = x2 - x1;
        double dy = y2 - y1;
        double distsqr = dx*dx + dy*dy;
        
        if (distsqr >= THREESYMBRADSQR) {
            cairo_move_to(cr, x2+SYMBRAD, y2);
            cairo_arc(cr, x2, y2, SYMBRAD, 0.0, 6.283185);
            if (self->fill_symbol) cairo_fill(cr);
            x1 = x2;
            y1 = y2;
        }
    }
    cairo_stroke(cr);
}


void _slope_xydata_draw_triangles (slope_data_t *data, cairo_t *cr,
                                 slope_metrics_t *metrics)
{
    slope_xydata_t *self = (slope_xydata_t*) data;
    
    const double *vx = self->vx;
    const double *vy = self->vy;
    const int n = self->n;
    
    double x1 = slope_xymetrics_map_x(metrics, vx[0]);
    double y1 = slope_xymetrics_map_y(metrics, vy[0]);
    cairo_move_to(cr, x1-SYMBRAD, y1+SYMBRAD);
    cairo_line_to(cr, x1+SYMBRAD, y1+SYMBRAD);
    cairo_line_to(cr, x1, y1-SYMBRAD);
    cairo_close_path(cr);
    if (self->fill_symbol) cairo_fill(cr);
    
    int k;
    for (k=1; k<n; k++) {
        double x2 = slope_xymetrics_map_x(metrics, vx[k]);
        double y2 = slope_xymetrics_map_y(metrics, vy[k]);
        
        double dx = x2 - x1;
        double dy = y2 - y1;
        double distsqr = dx*dx + dy*dy;
        
        if (distsqr >= THREESYMBRADSQR) {
            cairo_move_to(cr, x2-SYMBRAD, y2+SYMBRAD);
            cairo_line_to(cr, x2+SYMBRAD, y2+SYMBRAD);
            cairo_line_to(cr, x2, y2-SYMBRAD);
            cairo_close_path(cr);
            if (self->fill_symbol) cairo_fill(cr);
            x1 = x2;
            y1 = y2;
        }
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
                       const int n,
                       const char *name,
                       slope_color_name_t color,
                       slope_scatter_t scatter)
{
    slope_xydata_t *self = (slope_xydata_t*) data;
    self->vx = vx;
    self->vy = vy;
    self->n = n;
    if (data->name) {
        free(data->name);
    }
    data->name = strdup(name);
    slope_color_set_name(&self->color, color);
    self->scatter = scatter;
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
