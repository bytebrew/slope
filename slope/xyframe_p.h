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

#ifndef _SLOPE_XYFRAME_P_H_
#define _SLOPE_XYFRAME_P_H_

#include "slope/xyframe.h"
#include "slope/frame_p.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _slope_xyframe slope_xyframe_t;

struct _slope_xyframe
{
    slope_frame_t parent;
    slope_color_t color;
    int visible_elements;
    
    double hdivlen, vdivlen;
};


slope_frame_t* _slope_xyframe_create (slope_metrics_t *metrics);

void _slope_xyframe_setup_draw (slope_frame_t *frame);

void _slope_xyframe_draw (slope_frame_t *frame, cairo_t *cr);

void _slope_xyframe_draw_bottom_top (slope_frame_t *frame, cairo_t *cr);

void _slope_xyframe_draw_left_right (slope_frame_t *frame, cairo_t *cr);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_XYFRAME_P_H_*/
