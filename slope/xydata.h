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

#ifndef _SLOPE_XYDATA_H_
#define _SLOPE_XYDATA_H_

#include "slope/data.h"
#include "slope/primitives.h"
#include "slope/xymetrics.h"

#ifdef __cplusplus
extern "C" {
#endif

slope_data_t*
slope_xydata_create_simple (double *vx, double *vy,
                            const int n,
                            const char *name,
                            slope_color_name_t color,
                            slope_scatter_t scatter);

void slope_xydata_set (slope_data_t *data,
                       double *vx, double *vy,
                       const int n);

double slope_xydata_x_max (slope_data_t *data);

double slope_xydata_x_min (slope_data_t *data);

double slope_xydata_y_max (slope_data_t *data);

double slope_xydata_y_min (slope_data_t *data);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_XYDATA_H_*/
