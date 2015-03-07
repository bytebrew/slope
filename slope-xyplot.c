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

#include "slope-xyplot_p.h"
#include "slope-cartesian_p.h"
#include <stdlib.h>

slope_xyplot_t*
slope_xyplot_create_simple (double *vx, double *vy, unsigned long n,
                             slope_color_name_t color,
                             slope_xyplot_symbol_t symbol)
{
    slope_xyplot_t *scat = malloc(sizeof(slope_xyplot_t));
    scat->visib = 1;
    scat->symb = symbol;
    slope_color_set_by_name(&scat->line_color, color);
    scat->vx = vx;
    scat->vy = vy;
    scat->n = n;
    _slope_xyplot_check_ranges(scat);
    return scat;
}

void slope_xyplot_destroy (slope_xyplot_t *xyplot)
{
    if (xyplot == NULL) {
        return;
    }
    free(xyplot);
    xyplot = NULL;
}

int slope_xyplot_visible (slope_xyplot_t *xyplot)
{
    return xyplot->visib;
}

void _slope_xyplot_draw (slope_xyplot_t *xyplot,
                          slope_metrics_t *cartesian,
                          cairo_t *cr)
{
    double *vx = xyplot->vx;
    double *vy = xyplot->vy;
    long k, n = xyplot->n;
    double x1 = slope_cartesian_map_x(cartesian, vx[0]);
    double y1 = slope_cartesian_map_y(cartesian, vy[0]);
    cairo_set_source_rgba(cr,
                          xyplot->line_color.red,
                          xyplot->line_color.green,
                          xyplot->line_color.blue,
                          xyplot->line_color.alpha);
    cairo_move_to(cr, x1, y1);
    for (k=1; k<n; k++) {
        double x2 = slope_cartesian_map_x(cartesian, vx[k]);
        double y2 = slope_cartesian_map_y(cartesian, vy[k]);
        cairo_line_to(cr, x2, y2);
    }
    cairo_stroke(cr);
}

void _slope_xyplot_check_ranges (slope_xyplot_t *xyplot)
{
    double *vx = xyplot->vx;
    double *vy = xyplot->vy;
    long k, n = xyplot->n;
    xyplot->x_min = xyplot->x_max = vx[0];
    xyplot->y_min = xyplot->y_max = vy[0];
    for (k=1; k<n; k++) {
        if (vx[k] < xyplot->x_min) xyplot->x_min = vx[k];
        if (vx[k] > xyplot->x_max) xyplot->x_max = vx[k];
        if (vy[k] < xyplot->y_min) xyplot->y_min = vy[k];
        if (vy[k] > xyplot->y_max) xyplot->y_max = vy[k];
    }
}

/* slope-xyplot.c */