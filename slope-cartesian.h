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

#ifndef _SLOPE_CARTESIAN_H_
#define _SLOPE_CARTESIAN_H_

#include "slope-metrics.h"
#include "slope-xyplot.h"
#include "slope-xyaxis.h"
#include "slope-list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _slope_xyplot slope_xyplot_t;

slope_metrics_t* slope_cartesian_create ();

double slope_cartesian_map_x (slope_metrics_t *cart, double x);

double slope_cartesian_map_y (slope_metrics_t *cart, double y);

void slope_cartesian_add_xyplot (slope_metrics_t *cart, slope_xyplot_t *xyplot);

void slope_cartesian_rescale (slope_metrics_t *cart);

slope_xyaxis_t* slope_cartesian_get_axis (slope_metrics_t *cart,
                                          slope_xyaxis_type_t type);

slope_list_t* slope_cartesian_plot_list (slope_metrics_t *cart);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_CARTESIAN_H_*/