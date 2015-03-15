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

#include "slope/legend_p.h"
#include "slope/metrics_p.h"
#include "slope/data_p.h"
#include <stdlib.h>
#include <stdio.h>


slope_legend_t* _slope_legend_create(slope_metrics_t *metrics)
{
    slope_legend_t *legend = malloc(sizeof(slope_legend_t));
    legend->visible = SLOPE_TRUE;
    slope_color_set_name(&legend->stroke_color, SLOPE_BLACK);
    slope_color_set(&legend->fill_color, 0.9, 0.9, 0.9, 1.0);
    legend->metrics = metrics;
    return legend;
}


void _slope_legend_destroy (slope_legend_t *legend)
{
    if (legend == NULL) {
        return;
    }
    free(legend);
    legend = NULL;
}


void _slope_legend_guess_geometry (slope_legend_t *legend,
                                  cairo_t *cr, slope_point_t *point)
{
    legend->nlines = 0;
    double bgst_width = 0.0;
    slope_rect_set(
        &legend->rect, point->x, point->y, 36.0, 12.0);

    slope_iterator_t *iter =
        slope_list_first(legend->metrics->data);
    while (iter) {
        slope_data_t *data = slope_iterator_data(iter);
        
        if (data->visible == SLOPE_FALSE ||
            data->has_thumb == SLOPE_FALSE)
        {
            slope_iterator_next(&iter);
            continue;
        }
        legend->nlines += 1;
        cairo_text_extents_t txt_ext;
        cairo_text_extents(cr, data->name, &txt_ext);
        if (txt_ext.width > bgst_width)
            bgst_width = txt_ext.width;
        legend->rect.height += txt_ext.height + 5.0;
        
        slope_iterator_next(&iter);
    }
    legend->rect.width += bgst_width;
}


void _slope_legend_draw (slope_legend_t *legend,
                         cairo_t *cr, slope_point_t *point)
{
    if (legend->visible == SLOPE_FALSE)
        return;

    _slope_legend_guess_geometry(legend,cr,point);
    
    if (legend->nlines == 0)
        return;
    
    /* draw the legend's frame */
    slope_cairo_set_color(cr, &legend->fill_color);
    /*cairo_move_to(cr,legend->rect.x, legend->rect.y);*/
    slope_cairo_rectangle(cr, &legend->rect);
    cairo_fill(cr);
    cairo_stroke(cr);
    
    slope_cairo_set_color(cr, &legend->stroke_color);
    slope_cairo_rectangle(cr, &legend->rect);
    cairo_stroke(cr);
    
    /* draw data entries */
    int order = 1;
    cairo_text_extents_t txt_ext;
    cairo_text_extents(cr, "SAMPLE", &txt_ext);
    slope_point_t thumb_pos;
    thumb_pos.x = legend->rect.x + 15.0;
    
    slope_iterator_t *iter =
        slope_list_first(legend->metrics->data);
    while (iter) {
        slope_data_t *data = slope_iterator_data(iter);
        if (data->visible == SLOPE_FALSE ||
            data->has_thumb == SLOPE_FALSE)
        {
            slope_iterator_next(&iter);
            continue;
        }
        
        thumb_pos.y = legend->rect.y + (order++)*(txt_ext.height + 6.0);
        _slope_data_draw_thumb(data, cr, &thumb_pos);
        slope_iterator_next(&iter);
    }
}


int slope_legend_visible (slope_legend_t *legend)
{
    if (legend == NULL) {
        return SLOPE_FALSE;
    }
    return legend->visible;
}


int slope_legend_set_visible (slope_legend_t *legend,
                              int visible)
{
    if (legend == NULL) {
        return;
    }
    legend->visible = visible;
}

/* slope/legend.c */
