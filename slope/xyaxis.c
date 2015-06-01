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

#include "slope/xyaxis_p.h"
#include "slope/xymetrics_p.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


slope_item_class_t* __slope_xyaxis_get_class()
{
    static int first_call = SLOPE_TRUE;
    static slope_item_class_t klass;

    if (first_call) {
        klass.destroy_fn = NULL;
        klass.draw_fn = __slope_xyaxis_draw;
        klass.draw_thumb_fn = NULL;
        first_call = SLOPE_FALSE;
    }

    return &klass;
}


slope_item_t* slope_xyaxis_create (slope_metrics_t *metrics,
                                   slope_xyaxis_type_t type,
                                   const char *name)
{
    slope_xyaxis_t *axis = malloc(sizeof(slope_xyaxis_t));
    slope_item_t *parent = (slope_item_t*) axis;

    axis->type = type;
    slope_color_set_name(&axis->color, SLOPE_BLACK);
    parent->name = strdup(name);
    parent->visible = SLOPE_TRUE;
    parent->has_thumb = SLOPE_FALSE;
    parent->metrics = metrics;
    parent->klass = __slope_xyaxis_get_class();

    return parent;
}


void __slope_xyaxis_setup_draw (slope_item_t *item, cairo_t *cr,
                                const slope_metrics_t *metrics)
{
    slope_xyaxis_t *axis = (slope_xyaxis_t*) item;
    const slope_xymetrics_t *xymetr = (const slope_xymetrics_t*) metrics;

    switch (axis->type) {
        case SLOPE_XYAXIS_TOP:
            axis->length = metrics->width_figure;
            axis->divlen = xymetr->xmax - xymetr->xmin;
            axis->divnum = axis->length / 70.0;
            break;
        case SLOPE_XYAXIS_BOTTOM:
            axis->length = metrics->width_figure;
            axis->divlen = xymetr->xmax - xymetr->xmin;
            axis->divnum = axis->length / 70.0;
            break;
        case SLOPE_XYAXIS_LEFT:
            axis->length = metrics->height_figure;
            axis->divlen = xymetr->ymax - xymetr->ymin;
            axis->divnum = axis->length / 50.0;
            break;
        case SLOPE_XYAXIS_RIGHT:
            axis->length = metrics->height_figure;
            axis->divlen = xymetr->ymax - xymetr->ymin;
            axis->divnum = axis->length / 50.0;
            break;
    }
    axis->divlen /= axis->divnum;
    axis->divnum *= 5.0;
    axis->divlen /= 5.0;
}


void __slope_xyaxis_draw (slope_item_t *item, cairo_t *cr,
                          const slope_metrics_t *metrics)
{
    slope_xyaxis_t *axis = (slope_xyaxis_t*) item;

    slope_cairo_set_color(cr, &axis->color);
    cairo_set_line_width(cr, 1.0);
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

    __slope_xyaxis_setup_draw(item, cr, metrics);

    switch (axis->type) {
        case SLOPE_XYAXIS_TOP:
            __slope_xyaxis_draw_top(item, cr, metrics);
            break;
        case SLOPE_XYAXIS_BOTTOM:
            __slope_xyaxis_draw_bottom(item, cr, metrics);
            break;
        case SLOPE_XYAXIS_LEFT:
            __slope_xyaxis_draw_left(item, cr, metrics);
            break;
        case SLOPE_XYAXIS_RIGHT:
            __slope_xyaxis_draw_right(item, cr, metrics);
            break;
    }
}


void __slope_xyaxis_draw_top (slope_item_t *item, cairo_t *cr,
                              const slope_metrics_t *metrics)
{
    slope_xyaxis_t *axis = (slope_xyaxis_t*) item;
    const slope_xymetrics_t *xymetr = (const slope_xymetrics_t*) metrics;

    double x = metrics->xmin_figure;
    double y = metrics->ymin_figure;
    double coord = xymetr->xmin;
    char label[32];

    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x+axis->length, y);

    int k;
    for (k=0; k<=axis->divnum; k++) {
        cairo_move_to(cr, x, y);
        if (k%5 == 0) {
            sprintf(label, "%2.2lf", coord);
            cairo_text_extents_t txt_ext;
            cairo_text_extents(cr, label, &txt_ext);
            cairo_line_to(cr, x, y+8.0);
            cairo_move_to(cr, x-txt_ext.width/2, y-txt_ext.height);
            cairo_show_text(cr, label);
        }
        else {
            cairo_line_to(cr, x, y+4.0);
        }
        coord += axis->divlen;
        x = slope_xymetrics_map_x(metrics, coord);
    }
    sprintf(label, "%s", item->name);
    cairo_text_extents_t txt_ext;
    cairo_text_extents(cr, item->name, &txt_ext);
    x = metrics->xmin_figure + (metrics->width_figure - txt_ext.width)/2.0;
    y = y - 3.0*txt_ext.height;
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, item->name);
    
    cairo_stroke(cr);
}


void __slope_xyaxis_draw_bottom (slope_item_t *item, cairo_t *cr,
                                 const slope_metrics_t *metrics)
{
    slope_xyaxis_t *axis = (slope_xyaxis_t*) item;
    const slope_xymetrics_t *xymetr = (const slope_xymetrics_t*) metrics;

    double x = metrics->xmin_figure;
    double y = metrics->ymax_figure;
    double coord = xymetr->xmin;
    char label[32];

    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x+axis->length, y);

    int k;
    for (k=0; k<=axis->divnum; k++) {
        cairo_move_to(cr, x, y);
        if (k%5 == 0) {
            sprintf(label, "%2.2lf", coord);
            cairo_text_extents_t txt_ext;
            cairo_text_extents(cr, label, &txt_ext);
            cairo_line_to(cr, x, y-8.0);
            cairo_move_to(cr, x-txt_ext.width/2, y+2*txt_ext.height);
            cairo_show_text(cr, label);
        }
        else {
            cairo_line_to(cr, x, y-4.0);
        }
        coord += axis->divlen;
        x = slope_xymetrics_map_x(metrics, coord);
    }
    sprintf(label, "%s", item->name);
    cairo_text_extents_t txt_ext;
    cairo_text_extents(cr, item->name, &txt_ext);
    x = metrics->xmin_figure + (metrics->width_figure - txt_ext.width)/2.0;
    y = y + 3.2*txt_ext.height;
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, item->name);
    
    cairo_stroke(cr);
}


void __slope_xyaxis_draw_left (slope_item_t *item, cairo_t *cr,
                               const slope_metrics_t *metrics)
{
    slope_xyaxis_t *axis = (slope_xyaxis_t*) item;
    const slope_xymetrics_t *xymetr = (const slope_xymetrics_t*) metrics;

    double x = metrics->xmin_figure;
    double y = metrics->ymax_figure;
    double coord = xymetr->ymin;
    char label[32];
    double max_txt_wid = 0.0;
    
    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x, y-axis->length);

    int k;
    for (k=0; k<=axis->divnum; k++) {
        cairo_move_to(cr, x, y);
        if (k%5 == 0) {
            sprintf(label, "%2.2lf", coord);
            cairo_text_extents_t txt_ext;
            cairo_text_extents(cr, label, &txt_ext);
            if (txt_ext.width > max_txt_wid) max_txt_wid = txt_ext.width;
            cairo_line_to(cr, x+8.0, y);
            cairo_move_to(
                cr, x-txt_ext.width-txt_ext.height, y+0.5*txt_ext.height);
            cairo_show_text(cr, label);
        }
        else {
            cairo_line_to(cr, x+4.0, y);
        }
        coord += axis->divlen;
        y = slope_xymetrics_map_y(metrics, coord);
    }
    cairo_save(cr);
    cairo_rotate(cr, -M_PI/2.0);
    sprintf(label, "%s", item->name);
    cairo_text_extents_t txt_ext;
    cairo_text_extents(cr, item->name, &txt_ext);
    x = - metrics->ymin_figure - (metrics->height_figure + txt_ext.width)/2.0;
    y = metrics->xmin_figure - max_txt_wid - 2.0*txt_ext.height;
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, item->name);
    cairo_restore(cr);
    cairo_stroke(cr);
}


void __slope_xyaxis_draw_right (slope_item_t *item, cairo_t *cr,
                                const slope_metrics_t *metrics)
{
    slope_xyaxis_t *axis = (slope_xyaxis_t*) item;
    const slope_xymetrics_t *xymetr = (const slope_xymetrics_t*) metrics;

    double x = metrics->xmax_figure;
    double y = metrics->ymax_figure;
    double coord = xymetr->ymin;
    char label[32];
    double max_txt_wid = 0.0;

    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x, y-axis->length);

    int k;
    for (k=0; k<=axis->divnum; k++) {
        cairo_move_to(cr, x, y);
        if (k%5 == 0) {
            sprintf(label, "%2.2lf", coord);
            cairo_text_extents_t txt_ext;
            cairo_text_extents(cr, label, &txt_ext);
            if (txt_ext.width > max_txt_wid) max_txt_wid = txt_ext.width;
            cairo_line_to(cr, x-8.0, y);
            cairo_move_to(cr, x+txt_ext.height, y+0.5*txt_ext.height);
            cairo_show_text(cr, label);
        }
        else {
            cairo_line_to(cr, x-4.0, y);
        }
        coord += axis->divlen;
        y = slope_xymetrics_map_y(metrics, coord);
    }
    cairo_save(cr);
    cairo_rotate(cr, -M_PI/2.0);
    sprintf(label, "%s", item->name);
    cairo_text_extents_t txt_ext;
    cairo_text_extents(cr, item->name, &txt_ext);
    x = - metrics->ymin_figure - (metrics->height_figure + txt_ext.width)/2.0;
    y = metrics->xmax_figure + max_txt_wid + 2.6*txt_ext.height;
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, item->name);
    cairo_restore(cr);
    cairo_stroke(cr);
}


slope_xyaxis_type_t slope_xyaxis_get_type (slope_item_t *item)
{
    if (item == NULL) {
        return SLOPE_XYAXIS_BOTTOM;
    }
    slope_xyaxis_t *axis = (slope_xyaxis_t*) item;
    return axis->type;
}

/* slope/xyaxis.c */
