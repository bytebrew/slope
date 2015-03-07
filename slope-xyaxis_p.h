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

#ifndef _SLOPE_XYAXIS_P_H_
#define _SLOPE_XYAXIS_P_H_

#include "slope-xyaxis.h"
#include "slope-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _slope_xyaxis
{
    slope_xyaxis_type_t type;
    int visib;
    
    int divnum;
    double length;
    double divsiz;
};


void _slope_xyaxis_draw (slope_xyaxis_t *axis,
                         slope_metrics_t *cartesian,
                         cairo_t *cr);

void _slope_xyaxis_setup_draw (slope_xyaxis_t *axis,
                               slope_metrics_t *cartesian);

void _slope_xyaxis_draw_bottom(slope_xyaxis_t *axis,
                               slope_metrics_t *cartesian,
                               cairo_t *cr);

void _slope_xyaxis_draw_left(slope_xyaxis_t *axis,
                             slope_metrics_t *cartesian,
                             cairo_t *cr);

void _slope_xyaxis_draw_top(slope_xyaxis_t *axis,
                            slope_metrics_t *cartesian,
                            cairo_t *cr);

void _slope_xyaxis_draw_right(slope_xyaxis_t *axis,
                              slope_metrics_t *cartesian,
                              cairo_t *cr);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_XYAXIS_P_H_*/
