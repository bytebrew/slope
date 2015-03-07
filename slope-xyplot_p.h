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

#ifndef _SLOPE_XYPLOT_P_H_
#define _SLOPE_XYPLOT_P_H_

#include "slope-xyplot.h"

#ifdef __cplusplus
extern "C" {
#endif


struct _slope_xyplot
{
    int visib;
    slope_xyplot_symbol_t symb;
    slope_color_t line_color;
    double *vx, *vy;
    unsigned long n;
    double x_min, x_max;
    double y_min, y_max;
};


void _slope_xyplot_draw (slope_xyplot_t *xyplot,
                          slope_metrics_t *cartesian,
                          cairo_t *cr);

void _slope_xyplot_check_ranges(slope_xyplot_t *xyplot);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_XYPLOT_P_H_*/
