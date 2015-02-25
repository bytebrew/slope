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

#ifndef _SLOPE_SCALE_H_
#define _SLOPE_SCALE_H_

#include "slope-list.h"
#include "slope-primitives.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _slope_scale slope_scale_t;
typedef struct _slope_plotable slope_plotable_t;


slope_iterator_t* slope_scale_first_plotable (slope_scale_t *scale);

slope_list_t* slope_scale_plotable_list (slope_scale_t *scale);

slope_chart_t* slope_scale_chart (slope_scale_t *scale);

void slope_scale_append_plotable (slope_scale_t *scale, slope_plotable_t *plot);

void slope_scale_get_chart_rect (slope_scale_t *scale, slope_rect_t *rect);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_SCALE_H_*/
