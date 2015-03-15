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

#ifndef _SLOPE_XYMETRICS_H_
#define _SLOPE_XYMETRICS_H_

#include "slope/metrics.h"
#include "slope/xyframe.h"

#ifdef __cplusplus
extern "C" {
#endif

slope_metrics_t* slope_xymetrics_create();

slope_frame_t* slope_xymetrics_get_frame (slope_metrics_t *metrics);

double slope_xymetrics_map_x (slope_metrics_t *metrics, double x);

double slope_xymetrics_map_y (slope_metrics_t *metrics, double y);

void slope_xymetrics_set_x_range (slope_metrics_t *metrics, double xi, double xf);

void slope_xymetrics_set_y_range (slope_metrics_t *metrics, double yi, double yf);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_XYMETRICS_H_*/

