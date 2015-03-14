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
#include <stdio.h>
#include <cairo.h>

#define TICKLEN 6


slope_frame_t* _slope_xyframe_create(slope_metrics_t *metrics)
{
    slope_xyframe_t *self = malloc(sizeof(slope_xyframe_t));
    slope_frame_t *parent = (slope_frame_t*) self;
    parent->visible = 1;
    parent->metrics = metrics;
    parent->_cleanup_fn = NULL;
    parent->_draw_fn = _slope_xyframe_draw;
    slope_xyframe_set_visible(parent, SLOPE_XYFRAME_ALL,
                              SLOPE_TRUE);
    slope_color_set_name(&self->color, SLOPE_BLACK);
    return parent;
}


void slope_xyframe_set_visible (slope_frame_t *frame,
                                slope_xyframe_element_t element,
                                int visible)
{
    slope_xyframe_t *self = (slope_xyframe_t*) frame;
    /* all at once */
    if (element == SLOPE_XYFRAME_ALL) {
        if (visible) {
            self->visible_elements =
                SLOPE_XYFRAME_TOP|SLOPE_XYFRAME_BOTTOM|
                SLOPE_XYFRAME_LEFT|SLOPE_XYFRAME_RIGHT|
                SLOPE_XYFRAME_GRID;
        }
        else {
            self->visible_elements = 0;
        }
        return;
    }
    /* one particular */
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
    
    _slope_xyframe_setup_draw(frame);
    _slope_xyframe_draw_bottom_top(frame,cr);
    _slope_xyframe_draw_left_right(frame,cr);
}


void _slope_xyframe_setup_draw (slope_frame_t *frame)
{
    slope_xyframe_t *self = (slope_xyframe_t*) frame;
    slope_xymetrics_t *metrics = (slope_xymetrics_t*) frame->metrics;
    
    double divnum = metrics->width_scene / 80.0;
    self->hdivlen = (metrics->xmax - metrics->xmin) / divnum;
    
    divnum = metrics->height_scene / 60.0;
    self->vdivlen = (metrics->ymax - metrics->ymin) / divnum;
}


void _slope_xyframe_draw_bottom_top (slope_frame_t *frame, cairo_t *cr)
{
    slope_xyframe_t *self = (slope_xyframe_t*) frame;
    slope_xymetrics_t *metrics = (slope_xymetrics_t*) frame->metrics;
    
    double x = metrics->xmin_scene;
    double yup = metrics->ymin_scene;
    double ydn = metrics->ymax_scene;
    double coord = metrics->xmin;
    char txt[32];
    
    if (self->visible_elements & SLOPE_XYFRAME_TOP) {
        cairo_move_to(cr,x,yup);
        cairo_line_to(cr,x+metrics->width_scene,yup);
    }
    if (self->visible_elements & SLOPE_XYFRAME_BOTTOM) {
        cairo_move_to(cr,x,ydn);
        cairo_line_to(cr,x+metrics->width_scene,ydn);
    }
    cairo_stroke(cr);
    
    while (coord <= metrics->xmax) {
        sprintf(txt, "%2.2lf", coord);
        cairo_text_extents_t txt_ext;
        cairo_text_extents(cr, txt, &txt_ext);
        
        if (self->visible_elements & SLOPE_XYFRAME_TOP) {
            cairo_move_to(cr,x,yup);
            cairo_line_to(cr,x,yup+TICKLEN);
            cairo_move_to(cr,x-txt_ext.width/2.0,yup-txt_ext.height);
            cairo_show_text(cr,txt);
        }
        if (self->visible_elements & SLOPE_XYFRAME_BOTTOM) {
            cairo_move_to(cr,x,ydn);
            cairo_line_to(cr,x,ydn-TICKLEN);
            cairo_move_to(cr,x-txt_ext.width/2.0,ydn+2.0*txt_ext.height);
            cairo_show_text(cr,txt);
        }
        if (self->visible_elements & SLOPE_XYFRAME_GRID) {
            cairo_stroke(cr);
            cairo_save(cr);
            cairo_set_source_rgba(
                cr, self->color.red, self->color.green,
                self->color.blue, 0.20);
            cairo_set_line_width(cr, 1.0);
            cairo_move_to(cr,x,ydn);
            cairo_line_to(cr,x,ydn-metrics->height_scene);
            cairo_stroke(cr);
            cairo_restore(cr);
        }
        coord += self->hdivlen;
        x = slope_xymetrics_map_x(frame->metrics, coord);
    }
    cairo_stroke(cr);
}


void _slope_xyframe_draw_left_right (slope_frame_t *frame, cairo_t *cr)
{
    slope_xyframe_t *self = (slope_xyframe_t*) frame;
    slope_xymetrics_t *metrics = (slope_xymetrics_t*) frame->metrics;
    
    double xlf = metrics->xmin_scene;
    double xrt = metrics->xmax_scene;
    double y = metrics->ymax_scene;
    double coord = metrics->ymin;
    char txt[32];
    
    if (self->visible_elements & SLOPE_XYFRAME_LEFT) {
        cairo_move_to(cr,xlf,y);
        cairo_line_to(cr,xlf,y-metrics->height_scene);
    }
    if (self->visible_elements & SLOPE_XYFRAME_RIGHT) {
        cairo_move_to(cr,xrt,y);
        cairo_line_to(cr,xrt,y-metrics->height_scene);
    }

    cairo_stroke(cr);
    
    while (coord <= metrics->ymax) {
        sprintf(txt, "%2.2lf", coord);
        cairo_text_extents_t txt_ext;
        cairo_text_extents(cr, txt, &txt_ext);
        
        if (self->visible_elements & SLOPE_XYFRAME_LEFT) {
            cairo_move_to(cr,xlf,y);
            cairo_line_to(cr,xlf+TICKLEN,y);
            cairo_move_to(cr,xlf-txt_ext.height-txt_ext.width,
                          y+txt_ext.height/2.0);
            cairo_show_text(cr,txt);
        }
        if (self->visible_elements & SLOPE_XYFRAME_RIGHT) {
            cairo_move_to(cr,xrt,y);
            cairo_line_to(cr,xrt-TICKLEN,y);
            cairo_move_to(cr,xrt+txt_ext.height, y+txt_ext.height/2.0);
            cairo_show_text(cr,txt);
        }
        if (self->visible_elements & SLOPE_XYFRAME_GRID) {
            cairo_stroke(cr);
            cairo_save(cr);
            cairo_set_source_rgba(
                cr, self->color.red, self->color.green,
                self->color.blue, 0.20);
            cairo_set_line_width(cr, 1.0);
            cairo_move_to(cr,xlf,y);
            cairo_line_to(cr,xlf+metrics->width_scene,y);
            cairo_stroke(cr);
            cairo_restore(cr);
        }
        coord += self->vdivlen;
        y = slope_xymetrics_map_y(frame->metrics, coord);
    }
    cairo_stroke(cr);
}

/* slope/xyframe.c */
