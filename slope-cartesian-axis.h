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

#ifndef _SLOPE_CARTESIAN_AXIS_H_
#define _SLOPE_CARTESIAN_AXIS_H_

#include "slope-cartesian.h"

#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct _slope_cartesian_axis slope_cartesian_axis_t;


typedef enum
{
    SLOPE_AXIS_BOTTOM,
    SLOPE_AXIS_LEFT,
    SLOPE_AXIS_TOP,
    SLOPE_AXIS_RIGHT
}
slope_cartesian_axis_type_t;

slope_cartesian_axis_t* slope_cartesian_axis_create (slope_cartesian_axis_type_t type);

void slope_cartesian_axis_destroy (slope_cartesian_axis_t *axis);

int slope_cartesian_axis_visible (slope_cartesian_axis_t *axis);


#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_CARTESIAN_H_*/