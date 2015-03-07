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

#include "slope-xyaxis_p.h"
#include "slope-cartesian_p.h"
#include <stdlib.h>
#include <cairo.h>


#define TICKLEN 8


slope_xyaxis_t* slope_xyaxis_create (slope_xyaxis_type_t type)
{
    slope_xyaxis_t *axis = malloc(sizeof(slope_xyaxis_t));
    axis->type = type;
    axis->visib = 1;
    return axis;
}


void slope_xyaxis_destroy (slope_xyaxis_t *axis)
{
    if (axis == NULL) {
        return;
    }
    free(axis);
    axis = NULL;
}


void _slope_xyaxis_setup_draw (slope_xyaxis_t *axis,
                               slope_metrics_t *metrics)
{
    slope_cartesian_t *cartesian = (slope_cartesian_t*) metrics;
    switch (axis->type) {
        case SLOPE_AXIS_TOP:
            axis->length = cartesian->width_scene;
            axis->divnum = (int) (axis->length / 80.0);
            axis->divsiz = cartesian->x_max - cartesian->x_min;
            break;
        case SLOPE_AXIS_LEFT:
            axis->length = cartesian->height_scene;
            axis->divnum = (int) (axis->length / 60.0);
            axis->divsiz = cartesian->y_max - cartesian->y_min;
            break;
        case SLOPE_AXIS_BOTTOM:
            axis->length = cartesian->width_scene;
            axis->divnum = (int) (axis->length / 80.0);
            axis->divsiz = cartesian->x_max - cartesian->x_min;
            break;
        case SLOPE_AXIS_RIGHT:
            axis->length = cartesian->height_scene;
            axis->divnum = (int) (axis->length / 60.0);
            axis->divsiz = cartesian->y_max - cartesian->y_min;
            break;
    }
    axis->divsiz /= axis->divnum;
}


void _slope_xyaxis_draw (slope_xyaxis_t *axis,
                                slope_metrics_t *cartesian,
                                cairo_t *cr)
{
    _slope_xyaxis_setup_draw(axis, cartesian);
    
    cairo_set_line_width(cr, 1.0);
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    
    switch (axis->type) {
        case SLOPE_AXIS_TOP:
            _slope_xyaxis_draw_top(axis,cartesian,cr);
            break;
        case SLOPE_AXIS_LEFT:
            _slope_xyaxis_draw_left(axis,cartesian,cr);
            break;
        case SLOPE_AXIS_BOTTOM:
            _slope_xyaxis_draw_bottom(axis,cartesian,cr);
            break;
        case SLOPE_AXIS_RIGHT:
            _slope_xyaxis_draw_right(axis,cartesian,cr);
            break;
    }
}


void _slope_xyaxis_draw_bottom(slope_xyaxis_t *axis,
                               slope_metrics_t *metrics,
                               cairo_t *cr)
{
    slope_cartesian_t *cartesian = (slope_cartesian_t*) metrics;
    double x = cartesian->x_min_scene;
    double y = cartesian->y_max_scene;
    double coord = cartesian->x_min;
    
    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x+axis->length, y);
    cairo_stroke(cr);
    
    while (coord <= cartesian->x_max) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x, y - TICKLEN);
        coord += axis->divsiz;
        x = slope_cartesian_map_x(metrics, coord);
    }
    cairo_stroke(cr);
}


void _slope_xyaxis_draw_left(slope_xyaxis_t *axis,
                             slope_metrics_t *metrics,
                             cairo_t *cr)
{
    slope_cartesian_t *cartesian = (slope_cartesian_t*) metrics;
    double x = cartesian->x_min_scene;
    double y = cartesian->y_max_scene;
    double coord = cartesian->y_min;
    
    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x, y-axis->length);
    cairo_stroke(cr);
    
    while (coord <= cartesian->y_max) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x+TICKLEN, y);
        coord += axis->divsiz;
        y = slope_cartesian_map_y(metrics, coord);
    }
    cairo_stroke(cr);
}


void _slope_xyaxis_draw_top(slope_xyaxis_t *axis,
                            slope_metrics_t *metrics,
                            cairo_t *cr)
{
    slope_cartesian_t *cartesian = (slope_cartesian_t*) metrics;
    double x = cartesian->x_min_scene;
    double y = cartesian->y_min_scene;
    double coord = cartesian->x_min;
    
    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x+axis->length, y);
    cairo_stroke(cr);
    
    while (coord <= cartesian->x_max) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x, y + TICKLEN);
        coord += axis->divsiz;
        x = slope_cartesian_map_x(metrics, coord);
    }
    cairo_stroke(cr);
}


void _slope_xyaxis_draw_right(slope_xyaxis_t *axis,
                              slope_metrics_t *metrics,
                              cairo_t *cr)
{
    slope_cartesian_t *cartesian = (slope_cartesian_t*) metrics;
    double x = cartesian->x_max_scene;
    double y = cartesian->y_max_scene;
    double coord = cartesian->y_min;
    
    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x, y-axis->length);
    cairo_stroke(cr);
    
    while (coord <= cartesian->y_max) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x-TICKLEN, y);
        coord += axis->divsiz;
        y = slope_cartesian_map_y(metrics, coord);
    }
    cairo_stroke(cr);
}


int slope_xyaxis_visible (slope_xyaxis_t *axis)
{
    return axis->visib;
}

/* slope-xyaxis.c */