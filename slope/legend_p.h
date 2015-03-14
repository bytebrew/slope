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

#ifndef _SLOPE_LEGEND_P_H_
#define _SLOPE_LEGEND_P_H_

#include "slope/legend.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _slope_legend
{
    slope_rect_t rect;
    slope_color_t stroke_color;
    slope_color_t fill_color;
    int visible;
};


slope_legend_t* _slope_legend_create();

void _slope_legend_destroy (slope_legend_t *legend);


void _slope_legend_draw (slope_legend_t *legend, cairo_t *cr,
                         slope_point_t *point);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_LEGEND_P_H_*/
