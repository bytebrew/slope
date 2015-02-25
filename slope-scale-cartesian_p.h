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

#ifndef _SLOPE_SCALE_CARTESIAN_P_H_
#define _SLOPE_SCALE_CARTESIAN_P_H_

#include "slope-scale-cartesian.h"
#include "slope-scale_p.h"

#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct _slope_scale_cartesian slope_scale_cartesian_t;

struct _slope_scale_cartesian
{
    struct _slope_scale parent;

    double x_min, x_max;
    double y_min, y_max;
    double width, height;
};

void _slope_scale_cartesian_rescale (slope_scale_t *scale);

void _slope_scale_cartesian_cleanup (slope_scale_t *scale);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_SCALE_CARTESIAN_P_H_*/
