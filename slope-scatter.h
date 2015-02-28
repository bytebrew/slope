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

#ifndef _SLOPE_SCATTER_H_
#define _SLOPE_SCATTER_H_

#include "slope-cartesian.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _slope_scatter slope_scatter_t;

typedef enum _slope_scatter_symbol
{
    SLOPE_LINE,
    SLOPE_CIRCLES
}
slope_scatter_symbol_t;

slope_scatter_t* slope_scatter_create_for_data (double *vx, double *vy, unsigned long n,
                                                slope_scatter_symbol_t symbol);

void slope_scatter_destroy (slope_scatter_t *scatter);

int slope_scatter_visible (slope_scatter_t *scatter);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_SCATTER_H_*/