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

#ifndef _SLOPE_FRAME_P_H_
#define _SLOPE_FRAME_P_H_

#include "slope/frame.h"
#include "slope/metrics_p.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _slope_frame
{
    slope_metrics_t *metrics;
    int visible;

    void (*_cleanup_fn)(slope_frame_t*);
    void (*_draw_fn)(slope_frame_t*, cairo_t*);
};

void _slope_frame_destroy (slope_frame_t *frame);

void _slope_frame_draw (slope_frame_t *frame, cairo_t *cr);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_FRAME_P_H_*/
