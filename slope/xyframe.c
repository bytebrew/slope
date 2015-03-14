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

#include "slope/xyframe_p.h"
#include "slope/xymetrics_p.h"
#include <stdlib.h>
#include <cairo.h>


slope_frame_t* _slope_xyframe_create(slope_metrics_t *metrics)
{
    slope_xyframe_t *self = malloc(sizeof(slope_xyframe_t));
    slope_frame_t *parent = (slope_frame_t*) self;
    parent->visible = 1;
    parent->metrics = metrics;
    parent->_cleanup_fn = NULL;
    parent->_draw_fn = _slope_xyframe_draw;
    self->visible_elements =
        SLOPE_XYFRAME_TOP|SLOPE_XYFRAME_BOTTOM|
        SLOPE_XYFRAME_LEFT|SLOPE_XYFRAME_RIGHT;
    slope_color_set_name(&self->color, SLOPE_BLACK);
    return parent;
}


void slope_xyframe_set_element_visibility(slope_frame_t *frame,
                                          slope_xyframe_element_t element,
                                          int visible)
{
    slope_xyframe_t *self = (slope_xyframe_t*) frame;
    if (visible) {
        self->visible_elements = self->visible_elements|element;
    }
    else if (self->visible_elements & element) {
        self->visible_elements = self->visible_elements^element;
    }
}


void _slope_xyframe_draw (slope_frame_t *frame, cairo_t *cr)
{
    slope_xyframe_t *self = (slope_xyframe_t*) frame;
    cairo_set_source_rgba(
        cr, self->color.red, self->color.green, self->color.blue,
        self->color.alpha);
    cairo_set_line_width(cr, 1.0);
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
    
    _slope_xyframe_draw_bottom_top(frame,cr);
    _slope_xyframe_draw_left_right(frame,cr);
}


void _slope_xyframe_draw_bottom_top (slope_frame_t *frame, cairo_t *cr)
{
    slope_xyframe_t *self = (slope_xyframe_t*) frame;
    slope_xymetrics_t *metrics = (slope_xymetrics_t*) frame->metrics;
    
    double xup = metrics->xmin_scene;
    double yup = metrics->ymin_scene;
    double xdn = metrics->xmin_scene;
    double ydn = metrics->ymax_scene;
    
    if (self->visible_elements & SLOPE_XYFRAME_TOP) {
        cairo_move_to(cr,xup,yup);
        cairo_line_to(cr,xup+metrics->width_scene,yup);
    }
    if (self->visible_elements & SLOPE_XYFRAME_BOTTOM) {
        cairo_move_to(cr,xdn,ydn);
        cairo_line_to(cr,xdn+metrics->width_scene,ydn);
    }
    cairo_stroke(cr);
}


void _slope_xyframe_draw_left_right (slope_frame_t *frame, cairo_t *cr)
{
    slope_xyframe_t *self = (slope_xyframe_t*) frame;
    slope_xymetrics_t *metrics = (slope_xymetrics_t*) frame->metrics;
    
    double xlf = metrics->xmin_scene;
    double ylf = metrics->ymax_scene;
    double xrt = metrics->xmax_scene;
    double yrt = metrics->ymax_scene;
    
    if (self->visible_elements & SLOPE_XYFRAME_LEFT) {
        cairo_move_to(cr,xlf,ylf);
        cairo_line_to(cr,xlf,ylf-metrics->height_scene);
    }
    if (self->visible_elements & SLOPE_XYFRAME_RIGHT) {
        cairo_move_to(cr,xrt,yrt);
        cairo_line_to(cr,xrt,yrt-metrics->height_scene);
    }
    cairo_stroke(cr);
}

/* slope/xyframe.c */
