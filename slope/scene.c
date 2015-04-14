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

#include "slope/scene_p.h"
#include "slope/metrics_p.h"
#include "slope/list.h"
#include <cairo.h>
#include <stdlib.h>

slope_scene_t* slope_scene_create()
{
    slope_scene_t *scene = malloc(sizeof(slope_scene_t));
    scene->metrics = NULL;
    scene->change_callback = NULL;
    slope_color_set_name(&scene->back_color, SLOPE_WHITE);
    scene->fill_back = SLOPE_TRUE;
    return scene;
}


void slope_scene_destroy (slope_scene_t *scene)
{
    if (scene == NULL) {
        return;
    }
    slope_list_destroy(scene->metrics);
    free(scene);
}


void slope_scene_add_metrics (slope_scene_t *scene,
                              slope_metrics_t *metrics)
{
    metrics->scene = scene;
    scene->metrics = slope_list_append(scene->metrics,
                                       metrics);
    if (scene->change_callback) {
        (*scene->change_callback)(NULL);
    }
}


slope_list_t* slope_scene_get_metrics_list (slope_scene_t *scene)
{
    if (scene == NULL) {
        return NULL;
    }
    return scene->metrics;
}


void slope_scene_draw (slope_scene_t *scene, cairo_t *cr,
                       const slope_rect_t *rect)
{
    cairo_stroke(cr);
    cairo_save(cr);
    cairo_select_font_face(
        cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 11);
    slope_cairo_rectangle(cr, rect);
    cairo_clip(cr);
    /* fill background */
    if (scene->fill_back) {
        slope_cairo_set_color(cr, &scene->back_color);
        cairo_paint(cr);
    }
    cairo_stroke(cr);

    /* draw contents */
    slope_iterator_t *met_iter =
        slope_list_first(scene->metrics);
    while (met_iter) {
        slope_metrics_t *met = (slope_metrics_t*)
            slope_iterator_data(met_iter);
        if (slope_metrics_get_visible(met)) {
            __slope_metrics_draw(met, cr, rect);
        }
        slope_iterator_next(&met_iter);
    }
    cairo_restore(cr);
}


void slope_scene_write_to_png (slope_scene_t *scene,
                               const char *filename,
                               int width, int height)
{
    cairo_surface_t *surf = cairo_image_surface_create(
        CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surf);
    slope_rect_t rect;
    slope_rect_set(&rect, 0.0, 0.0, width, height);
    slope_scene_draw(scene, cr, &rect);
    cairo_surface_write_to_png(surf, filename);
    cairo_destroy(cr);
    cairo_surface_destroy(surf);
}


void slope_scene_set_change_callback (slope_scene_t *scene,
                                      slope_callback_t callback)
{
    if (scene == NULL) {
        return;
    }
    scene->change_callback = callback;
}

/* slope/scene.h */
