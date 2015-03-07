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

#ifndef _SLOPE_XYPLOT_H_
#define _SLOPE_XYPLOT_H_

#include "slope-cartesian.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _slope_xyplot slope_xyplot_t;

typedef enum _slope_xyplot_symbol
{
    SLOPE_LINE,
    SLOPE_CIRCLES
}
slope_xyplot_symbol_t;

slope_xyplot_t*
slope_xyplot_create_simple (double *vx, double *vy, unsigned long n,
                             slope_color_name_t color,
                             slope_xyplot_symbol_t symbol);

void slope_xyplot_destroy (slope_xyplot_t *xyplot);

int slope_xyplot_visible (slope_xyplot_t *xyplot);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_XYPLOT_H_*/