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

#ifndef _SLOPE_XYAXIS_H_
#define _SLOPE_XYAXIS_H_

#include "slope-xyaxis.h"

#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct _slope_xyaxis slope_xyaxis_t;


typedef enum
{
    SLOPE_AXIS_BOTTOM,
    SLOPE_AXIS_LEFT,
    SLOPE_AXIS_TOP,
    SLOPE_AXIS_RIGHT
}
slope_xyaxis_type_t;

slope_xyaxis_t* slope_xyaxis_create (slope_xyaxis_type_t type);

void slope_xyaxis_destroy (slope_xyaxis_t *axis);

int slope_xyaxis_visible (slope_xyaxis_t *axis);


#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_XYAXIS_H_*/