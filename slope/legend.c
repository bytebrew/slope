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
#include "slope/figure_p.h"
#include "slope/metrics_p.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


slope_item_class_t* __slope_legend_get_class()
{
    static int first_call = SLOPE_TRUE;
    static slope_item_class_t klass;

    if (first_call) {
        klass.destroy_fn = NULL;
        klass.draw_fn = __slope_legend_draw;
        first_call = SLOPE_FALSE;
    }

    return &klass;
}


slope_item_t* slope_legend_create ()
{
    slope_legend_t *legend = malloc(sizeof(slope_legend_t));
    slope_item_t *parent = (slope_item_t*) legend;

    parent->klass = __slope_legend_get_class();
    parent->visible = SLOPE_TRUE;
    legend->position = SLOPE_LEGEND_TOPRIGHT;

    slope_color_set_name(&legend->fill_color, SLOPE_WHITE);
    slope_color_set_name(&legend->stroke_color, SLOPE_BLACK);
    
    return parent;
}


void __slope_legend_eval_geometry (slope_item_t *item, cairo_t *cr,
                                   const slope_metrics_t *metrics)
{
    slope_legend_t *self = (slope_legend_t*) item;
    slope_figure_t *figure = slope_metrics_get_figure(metrics);
    
    double max_width = 0.0;
    self->rect.width = 0.0;
    self->rect.height = 0.0;

    slope_iterator_t *met_iter = slope_list_first(
        slope_figure_get_metrics_list(figure));
    /* for each metrics in the figure */
    while (met_iter) {
        slope_metrics_t *metrics = (slope_metrics_t*)
            slope_iterator_data(met_iter);
        slope_iterator_t *item_iter = slope_list_first(
            slope_metrics_get_item_list(metrics));
        /* for each item in the metrics */
        while (item_iter) {
            slope_item_t *item = (slope_item_t*)
                slope_iterator_data(item_iter);

                /* check if item is visible and has a legend thumb */
                if (slope_item_get_visible(item) == SLOPE_FALSE
                    || __slope_item_get_has_thumb(item) == SLOPE_FALSE) {
                        slope_iterator_next(&item_iter);
                        continue;
                }
                
                /* increment height and check for bigger width */
                cairo_text_extents_t txt_ext;
                cairo_text_extents(cr, slope_item_get_name(item), &txt_ext);
                self->rect.height += txt_ext.height + 4.0;
                if (txt_ext.width > max_width) max_width = txt_ext.width;
                
            slope_iterator_next(&item_iter);
        }
        slope_iterator_next(&met_iter);
    }
    
    self->rect.width = max_width + 40.0;
    self->rect.height += 10.0;
    
    switch (self->position) {
        case SLOPE_LEGEND_TOPRIGHT:
            self->rect.x = metrics->xmax_figure - self->rect.width - 10.0;
            self->rect.y = metrics->ymin_figure + 10.0;
            break;
        default: /* TOPRIGHT */
            self->rect.x = metrics->xmax_figure - self->rect.width - 10.0;
            self->rect.y = metrics->ymin_figure + 10.0;
            break;
    }
}


void __slope_legend_draw (slope_item_t *item, cairo_t *cr,
                          const slope_metrics_t *metrics)
{
    slope_legend_t *self = (slope_legend_t*) item;
    slope_figure_t *figure = slope_metrics_get_figure(metrics);
    slope_rect_t *rec = &self->rect;
    
    __slope_legend_eval_geometry(item, cr, metrics);
    
    /* fill background */
    slope_cairo_set_color(cr, &self->fill_color);
    cairo_rectangle(cr, rec->x, rec->y, rec->width, rec->height);
    cairo_fill_preserve(cr);
    
    /* paint outline */
    slope_cairo_set_color(cr, &self->stroke_color);
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
    cairo_set_line_width(cr, 1.0);
    cairo_stroke(cr);
    
    /* finaly draw the legend entries */
    const double x = self->rect.x + 35.0;
    double y = self->rect.y;
    
    slope_iterator_t *met_iter = slope_list_first(
        slope_figure_get_metrics_list(figure));
    /* for each metrics in the figure */
    while (met_iter) {
        slope_metrics_t *metrics = (slope_metrics_t*)
            slope_iterator_data(met_iter);
        slope_iterator_t *item_iter = slope_list_first(
            slope_metrics_get_item_list(metrics));
        /* for each item in the metrics */
        while (item_iter) {
            slope_item_t *item = (slope_item_t*)
                slope_iterator_data(item_iter);
            const char *entry = slope_item_get_name(item);
            
            /* check if item is visible and has a legend thumb */
            if (slope_item_get_visible(item) == SLOPE_FALSE
                || __slope_item_get_has_thumb(item) == SLOPE_FALSE) {
                    slope_iterator_next(&item_iter);
                    continue;
            }

            /* position pen and delegate entry drawing to the item */
            cairo_text_extents_t txt_ext;
            cairo_text_extents(cr, entry, &txt_ext);
            y += txt_ext.height + 4.0;
            cairo_move_to(cr, x, y);
            cairo_show_text(cr, entry);
            
            slope_iterator_next(&item_iter);
        }
        slope_iterator_next(&met_iter);
    }
    cairo_stroke(cr);
}

/* slope/legend.c */

