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
#include "slope/xymetrics_p.h"
#include <stdlib.h>
#include <string.h>

#define SYMBRAD 3.0
#define SYMBRADSQR 9.0
#define TWOSYMBRADSQR 36.0


slope_data_class_t* __slope_xydata_get_class()
{
    static int first_call = SLOPE_TRUE;
    static slope_data_class_t klass;

    if (first_call) {
        klass.destroy_fn = NULL;
        klass.draw_fn = __slope_xydata_draw;
        first_call = SLOPE_FALSE;
    }

    return &klass;
}


void __slope_xydata_init (slope_data_t *parent)
{
    slope_xydata_t *self = (slope_xydata_t*) parent;
    self->antialias = SLOPE_TRUE;
    self->line_width = 1.0;
    self->fill_symbol = SLOPE_TRUE;
    self->rescalable = SLOPE_TRUE;
    parent->name = NULL;
    parent->visible = SLOPE_TRUE;
    parent->has_thumb = SLOPE_TRUE;
    parent->metrics = NULL;
    parent->klass = __slope_xydata_get_class();
}


slope_data_t* slope_xydata_create()
{
    slope_xydata_t *self = malloc(sizeof(slope_xydata_t));
    slope_data_t *parent = (slope_data_t*) self;
    __slope_xydata_init(parent);
    return parent;
}


slope_data_t* slope_xydata_create_simple (const double *vx, const double *vy,
                                          const int n,
                                          const char *name,
                                          int line,
                                          const char *fmt)
{
    slope_xydata_t *self = malloc(sizeof(slope_xydata_t));
    slope_data_t *parent = (slope_data_t*) self;
    __slope_xydata_init(parent);
    slope_xydata_set(parent, vx, vy, n, name,line, fmt);
    return parent;
}


void slope_xydata_set (slope_data_t *data,
                       const double *vx, const double *vy,
                       const int n,
                       const char *name,
                       int line,
                       const char *fmt)
{
    slope_xydata_t *self = (slope_xydata_t*) data;
    self->vx = vx;
    self->vy = vy;
    self->n = n;
    if (data->name) {
        free(data->name);
    }
    data->name = strdup(name);
    slope_color_set_name(&self->color, __slope_data_parse_color(fmt));
    self->scatter = __slope_data_parse_scatter(fmt);
    self->line = line;
    __slope_xydata_check_ranges(data);
    slope_data_notify_data_change(data);
}


void slope_xydata_set_data (slope_data_t *data,
                            const double *vx, const double *vy,
                            const int n)
{
    slope_xydata_t *self = (slope_xydata_t*) data;
    self->vx = vx;
    self->vy = vy;
    self->n = n;
    __slope_xydata_check_ranges(data);
    slope_data_notify_data_change(data);
}


void slope_xydata_update_data (slope_data_t *data,
                               const double *vx, const double *vy,
                               const int n)
{
    slope_xydata_t *self = (slope_xydata_t*) data;
    self->vx = vx;
    self->vy = vy;
    self->n = n;
    slope_data_notify_appearence_change(data);
}


void __slope_xydata_draw (slope_data_t *data, cairo_t *cr,
                          const slope_metrics_t *metrics)
{
    slope_xydata_t *self = (slope_xydata_t*) data;

    slope_cairo_set_color(cr, &self->color);
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
        case SLOPE_CIRCLES:
            __slope_xydata_draw_circles(data, cr, metrics);
            break;
        case SLOPE_TRIANGLES:
            __slope_xydata_draw_triangles(data, cr, metrics);
            break;
        case SLOPE_SQUARES:
            __slope_xydata_draw_squares(data, cr, metrics);
            break;
        case SLOPE_PLUSSES:
            __slope_xydata_draw_plusses(data, cr, metrics);
            break;
    }
    if (self->line)
        __slope_xydata_draw_line(data, cr, metrics);
}


void __slope_xydata_draw_line (slope_data_t *data, cairo_t *cr,
                               const slope_metrics_t *metrics)
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


void __slope_xydata_draw_circles (slope_data_t *data, cairo_t *cr,
                                  const slope_metrics_t *metrics)
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

        if (distsqr >= TWOSYMBRADSQR) {
            cairo_move_to(cr, x2+SYMBRAD, y2);
            cairo_arc(cr, x2, y2, SYMBRAD, 0.0, 6.283185);
            if (self->fill_symbol) cairo_fill(cr);
            x1 = x2;
            y1 = y2;
        }
    }
    cairo_stroke(cr);
}


void __slope_xydata_draw_triangles (slope_data_t *data, cairo_t *cr,
                                    const slope_metrics_t *metrics)
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

        if (distsqr >= TWOSYMBRADSQR) {
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


void __slope_xydata_draw_squares (slope_data_t *data, cairo_t *cr,
                                  const slope_metrics_t *metrics)
{

}


void __slope_xydata_draw_plusses (slope_data_t *data, cairo_t *cr,
                                  const slope_metrics_t *metrics)
{
    slope_xydata_t *self = (slope_xydata_t*) data;
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

    const double *vx = self->vx;
    const double *vy = self->vy;
    const int n = self->n;

    double x1 = slope_xymetrics_map_x(metrics, vx[0]);
    double y1 = slope_xymetrics_map_y(metrics, vy[0]);
    cairo_move_to(cr, x1-SYMBRAD, y1);
    cairo_line_to(cr, x1+SYMBRAD, y1);
    cairo_move_to(cr, x1, y1-SYMBRAD);
    cairo_line_to(cr, x1, y1+SYMBRAD);
    int k;
    for (k=1; k<n; k++) {
        double x2 = slope_xymetrics_map_x(metrics, vx[k]);
        double y2 = slope_xymetrics_map_y(metrics, vy[k]);

        double dx = x2 - x1;
        double dy = y2 - y1;
        double distsqr = dx*dx + dy*dy;

        if (distsqr >= TWOSYMBRADSQR) {
            cairo_move_to(cr, x1-SYMBRAD, y1);
            cairo_line_to(cr, x1+SYMBRAD, y1);
            cairo_move_to(cr, x1, y1-SYMBRAD);
            cairo_line_to(cr, x1, y1+SYMBRAD);
            x1 = x2;
            y1 = y2;
        }
    }
    cairo_stroke(cr);
}


void __slope_xydata_check_ranges (slope_data_t *data)
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


void slope_xydata_set_antialias (slope_data_t *data, int on)
{
    if (data == NULL) {
        return;
    }
    slope_xydata_t *self = (slope_xydata_t*) data;
    self->antialias = on;
    slope_data_notify_appearence_change(data);
}

/* slope/xydata.c */

