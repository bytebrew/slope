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

#include "slope-cartesian-axis_p.h"
#include "slope-cartesian_p.h"
#include <stdlib.h>
#include <cairo.h>


static void _slope_cartesian_axis_draw_bottom(slope_cartesian_axis_t *axis,
                                              slope_plotable_t *cartesian,
                                              cairo_t *cr);
static void _slope_cartesian_axis_draw_left(slope_cartesian_axis_t *axis,
                                            slope_plotable_t *cartesian,
                                            cairo_t *cr);
static void _slope_cartesian_axis_draw_top(slope_cartesian_axis_t *axis,
                                           slope_plotable_t *cartesian,
                                           cairo_t *cr);
static void _slope_cartesian_axis_draw_right(slope_cartesian_axis_t *axis,
                                             slope_plotable_t *cartesian,
                                             cairo_t *cr);


slope_cartesian_axis_t* slope_cartesian_axis_create (slope_cartesian_axis_type_t type)
{
    slope_cartesian_axis_t *axis = malloc(sizeof(slope_cartesian_axis_t));
    axis->type = type;
    axis->visib = 1;
    return axis;
}


void slope_cartesian_axis_destroy (slope_cartesian_axis_t *axis)
{
    if (axis == NULL) {
        return;
    }
    free(axis);
    axis = NULL;
}


void _slope_cartesian_axis_draw (slope_cartesian_axis_t *axis,
                                slope_plotable_t *cartesian,
                                cairo_t *cr)
{
    cairo_set_line_width(cr, 1.0);
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    
    switch (axis->type) {
        case SLOPE_AXIS_TOP:
            _slope_cartesian_axis_draw_top(axis,cartesian,cr);
            break;
        case SLOPE_AXIS_LEFT:
            _slope_cartesian_axis_draw_left(axis,cartesian,cr);
            break;
        case SLOPE_AXIS_BOTTOM:
            _slope_cartesian_axis_draw_bottom(axis,cartesian,cr);
            break;
        case SLOPE_AXIS_RIGHT:
            _slope_cartesian_axis_draw_right(axis,cartesian,cr);
            break;
    }
}


static void _slope_cartesian_axis_draw_bottom(slope_cartesian_axis_t *axis,
                                              slope_plotable_t *cartesian_base,
                                              cairo_t *cr)
{
    slope_cartesian_t *cartesian = (slope_cartesian_t*) cartesian_base;
    double x = cartesian->x_min_scene;
    double y = cartesian->y_max_scene;
    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x+cartesian->width_scene, y);
    cairo_stroke(cr);
}


static void _slope_cartesian_axis_draw_left(slope_cartesian_axis_t *axis,
                                            slope_plotable_t *cartesian_base,
                                            cairo_t *cr)
{
    slope_cartesian_t *cartesian = (slope_cartesian_t*) cartesian_base;
    double x = cartesian->x_min_scene;
    double y = cartesian->y_max_scene;
    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x, y-cartesian->height_scene);
    cairo_stroke(cr);
}


static void _slope_cartesian_axis_draw_top(slope_cartesian_axis_t *axis,
                                           slope_plotable_t *cartesian_base,
                                           cairo_t *cr)
{
    slope_cartesian_t *cartesian = (slope_cartesian_t*) cartesian_base;
    double x = cartesian->x_min_scene;
    double y = cartesian->y_min_scene;
    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x+cartesian->width_scene, y);
    cairo_stroke(cr);
}


static void _slope_cartesian_axis_draw_right(slope_cartesian_axis_t *axis,
                                             slope_plotable_t *cartesian_base,
                                             cairo_t *cr)
{
    slope_cartesian_t *cartesian = (slope_cartesian_t*) cartesian_base;
    double x = cartesian->x_max_scene;
    double y = cartesian->y_max_scene;
    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x, y-cartesian->height_scene);
    cairo_stroke(cr);
}


int slope_cartesian_axis_visible (slope_cartesian_axis_t *axis)
{
    return axis->visib;
}
