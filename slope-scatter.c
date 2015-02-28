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

#include "slope-scatter_p.h"
#include "slope-cartesian_p.h"
#include <stdlib.h>
#include <stdio.h>

slope_scatter_t* slope_scatter_create_simple (double *vx, double *vy, unsigned long n,
                                             slope_scatter_symbol_t symbol,
                                             slope_color_name_t color)
{
    slope_scatter_t *scat = malloc(sizeof(slope_scatter_t));
    scat->visib = 1;
    scat->symb = symbol;
    slope_color_set_by_name(&scat->line_color, color);
    scat->vx = vx;
    scat->vy = vy;
    scat->n = n;
    _slope_scatter_check_ranges(scat);
    return scat;
}

void slope_scatter_destroy (slope_scatter_t *scatter)
{
    if (scatter == NULL) {
        return;
    }
    free(scatter);
    scatter = NULL;
}

int slope_scatter_visible (slope_scatter_t *scatter)
{
    return scatter->visib;
}

void _slope_scatter_draw (slope_scatter_t *scatter,
                          slope_plotable_t *cartesian,
                          cairo_t *cr)
{
    double *vx = scatter->vx;
    double *vy = scatter->vy;
    long k, n = scatter->n;
    double x1 = slope_cartesian_map_x(cartesian, vx[0]);
    double y1 = slope_cartesian_map_y(cartesian, vy[0]);
    cairo_set_source_rgba(cr,
                          scatter->line_color.red,
                          scatter->line_color.green,
                          scatter->line_color.blue,
                          scatter->line_color.alpha);
    cairo_move_to(cr, x1, y1);
    for (k=1; k<n; k++) {
        double x2 = slope_cartesian_map_x(cartesian, vx[k]);
        double y2 = slope_cartesian_map_y(cartesian, vy[k]);
        cairo_line_to(cr, x2, y2);
    }
    cairo_stroke(cr);
}

void _slope_scatter_check_ranges (slope_scatter_t *scatter)
{
    double *vx = scatter->vx;
    double *vy = scatter->vy;
    long k, n = scatter->n;
    scatter->x_min = scatter->x_max = vx[0];
    scatter->y_min = scatter->y_max = vy[0];
    for (k=1; k<n; k++) {
        if (vx[k] < scatter->x_min) scatter->x_min = vx[k];
        if (vx[k] > scatter->x_max) scatter->x_max = vx[k];
        if (vy[k] < scatter->y_min) scatter->y_min = vy[k];
        if (vy[k] > scatter->y_max) scatter->y_max = vy[k];
    }
}

/* slope-scatter.c */