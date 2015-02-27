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

#include "slope-cartesian.h" 
#include "slope-cartesian_p.h"
#include <stdlib.h>

slope_plotable_t* slope_cartesian_create ()
{
    slope_cartesian_t *cart = malloc(sizeof(slope_cartesian_t));
    slope_plotable_t *base = (slope_plotable_t*) cart;
    base->visib = 1;
    base->_cleanup_fn = _slope_cartesian_cleanup;
    base->_draw_fn = _slope_cartesian_draw;
    cart->scatters = NULL;
}

void _slope_cartesian_cleanup (slope_plotable_t *cartesian_base)
{
    slope_cartesian_t *cartesian = (slope_cartesian_t*) cartesian_base;
    slope_list_destroy(cartesian->scatters);
}

void _slope_cartesian_draw (slope_plotable_t *cartesian_base, cairo_t *cr, slope_rect_t *scene_rect)
{
    cairo_set_source_rgb(cr, 0,0,0);
    cairo_rectangle(cr,
                    scene_rect->x,
                    scene_rect->y,
                    scene_rect->width,
                    scene_rect->height);
    cairo_stroke(cr);
}

/* slope-cartesian.c */