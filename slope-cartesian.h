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

#include "slope-plotable.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _slope_scatter slope_scatter_t;

slope_plotable_t* slope_cartesian_create ();

double slope_cartesian_map_x (slope_plotable_t *cart, double x);

double slope_cartesian_map_y (slope_plotable_t *cart, double y);

void slope_cartesian_add_scatter (slope_plotable_t *cart, slope_scatter_t *scatter);

void slope_cartesian_rescale (slope_plotable_t *cart);


#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_CARTESIAN_H_*/