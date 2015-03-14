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

#include "slope/frame_p.h"
#include <stdlib.h>

void _slope_frame_destroy (slope_frame_t *frame)
{
    if (frame == NULL) {
        return;
    }
    if (frame->_cleanup_fn) {
        (*frame->_cleanup_fn)(frame);
    }
    free(frame);
    frame = NULL;
}


void slope_frame_set_visible(slope_frame_t *frame, int visible)
{
    if (frame == NULL) {
        return;
    }
    frame->visible = visible;
}


void _slope_frame_draw (slope_frame_t *frame, cairo_t *cr)
{
    if (frame->visible) {
        (*frame->_draw_fn)(frame, cr);
    }
}

/* slope/frame.c */
