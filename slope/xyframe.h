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

#ifndef _SLOPE_XYFRAME_H_
#define _SLOPE_XYFRAME_H_

#include "slope/frame.h"
#include "slope/metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    SLOPE_XYFRAME_TOP    = 1 << 0,
    SLOPE_XYFRAME_BOTTOM = 1 << 1,
    SLOPE_XYFRAME_LEFT   = 1 << 2,
    SLOPE_XYFRAME_RIGHT  = 1 << 3,
    SLOPE_XYFRAME_GRID   = 1 << 4,
    SLOPE_XYFRAME_ALL    = 1 << 5
}
slope_xyframe_element_t;


void slope_xyframe_set_visible (slope_frame_t *frame,
                                slope_xyframe_element_t element,
                                int visible);

void slope_xyframe_set_label (slope_frame_t *frame,
                              slope_xyframe_element_t element,
                              const char *label);

void slope_xyframe_set_color_name (slope_frame_t *frame,
                                   slope_color_name_t color);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_XYFRAME_H_*/
