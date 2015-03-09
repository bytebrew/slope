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

#ifndef _SLOPE_XYDATA_P_H_
#define _SLOPE_XYDATA_P_H_

#include "slope/xydata.h"
#include "slope/data_p.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _slope_xydata slope_xydata_t;

struct _slope_xydata
{
    struct _slope_data;
    
    double *vx, *vy;
    int n;
    double xmin, xmax;
    double ymin, ymax;
    
    slope_color_t color;
    slope_scatter_t scatter;
};

void _slope_xydata_set (slope_data_t *data,
                        double *vx, double *vy,
                        const int n);

void _slope_xydata_check_ranges (slope_data_t *data);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_XYDATA_P_H_*/

