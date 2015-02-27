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

#include "slope-chart.h"
#include "slope-chart_p.h"
#include "slope-plotable.h"
#include <stdlib.h>


/**
 * Creates a new chart
 */
slope_chart_t* slope_chart_create ()
{
    slope_chart_t *chart = malloc(sizeof(slope_chart_t));
    chart->plotables = NULL;
    chart->fill_back = 1;
    slope_color_set_by_name(&chart->back_color, SLOPE_WHITE);
    chart->x_low_b = chart->x_up_b = 20.0;
    chart->y_low_b = chart->y_up_b = 20.0;
    return chart;
}

/**
 * Drestroys chart
 */
void slope_chart_destroy (slope_chart_t *chart)
{
    slope_list_destroy(chart->plotables);
    free(chart);
    chart = NULL;
}

/**
 * Draw the contents of charts to the surface targeted by cr
 */
void slope_chart_draw (slope_chart_t *chart, cairo_t *cr, slope_rect_t *area)
{
    cairo_stroke(cr);
    if (chart->fill_back) {
        cairo_set_source_rgba(cr,
                              chart->back_color.red,
                              chart->back_color.green,
                              chart->back_color.blue,
                              chart->back_color.alpha);
        cairo_paint(cr);
    }
    slope_rect_t scene_rect;
    slope_rect_set(&scene_rect,
                   area->x + chart->x_low_b,
                   area->y + chart->y_low_b,
                   area->x + area->width - chart->x_up_b,
                   area->y + area->height - chart->y_up_b);
    cairo_rectangle(cr, scene_rect.x, scene_rect.y,
                    scene_rect.width, scene_rect.height);
    cairo_save(cr);
    cairo_clip(cr);
    slope_iterator_t *iter =
        slope_list_first(chart->plotables);
    while (iter) {
        slope_plotable_t *plot =
            (slope_plotable_t*) slope_iterator_data(iter);
        if (slope_plotable_visible (plot)) {
            slope_plotable_draw (plot, cr, &scene_rect);
        }
        slope_iterator_next(&iter);
    }
    cairo_restore(cr);
}

/*
 * Writes the chart to a png file
 */
void slope_chart_write_to_png (slope_chart_t *chart,
                               const char *filename,
                               int width, int height)
{
    cairo_surface_t *image =
        cairo_image_surface_create(CAIRO_FORMAT_RGB24,
                                   width, height);
    cairo_t *cr = cairo_create(image);
    slope_rect_t rect;
    slope_rect_set(&rect, 0.0, 0.0, width, height);
    slope_chart_draw(chart, cr, &rect);
    cairo_surface_write_to_png(image, filename);
    cairo_surface_destroy(image);
    cairo_destroy(cr);
}


void slope_chart_set_back_color_by_name (slope_chart_t *chart,
                                         slope_color_name_t color_name)
{
    slope_color_set_by_name(&chart->back_color, color_name);
}


/* slope-chart.c */