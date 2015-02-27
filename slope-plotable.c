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

#include "slope-plotable.h"
#include "slope-plotable_p.h"
#include <stdlib.h>


void slope_plotable_destroy (slope_plotable_t *plot)
{
    if (plot == NULL) {
        return;
    }
    if (plot->_cleanup_fn) {
        (*plot->_cleanup_fn)(plot);
    }
    free(plot);
    plot = NULL;
}

void slope_plotable_draw (slope_plotable_t *plot,
                          cairo_t *cr,
                          slope_rect_t *scene_rect)
{
    if (plot->_draw_fn) {
        (*plot->_draw_fn)(plot, cr, scene_rect);
    }
}

int slope_plotable_visible (slope_plotable_t *plot)
{
    return plot->visib;
}

/* slope-plotable.c */