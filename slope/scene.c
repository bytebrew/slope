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
#include "slope/data.h"
#include "slope/list.h"
#include <cairo.h>
#include <cairo-svg.h>
#include <cairo-pdf.h>
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
    /* use an easy font */
    cairo_select_font_face(cr, "Sans",
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
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


int slope_scene_write_to_png (slope_scene_t *scene,
                               const char *filename,
                               int width, int height)
{
    int success = SLOPE_TRUE;

    cairo_surface_t *surf = cairo_image_surface_create(
        CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surf);

    slope_rect_t rect;
    slope_rect_set(&rect, 0.0, 0.0, width, height);
    slope_scene_draw(scene, cr, &rect);
    if (cairo_surface_write_to_png(surf, filename) != CAIRO_STATUS_SUCCESS)
        success = SLOPE_FALSE;

    cairo_destroy(cr);
    cairo_surface_destroy(surf);

    return success;
}


int slope_scene_write_to_svg (slope_scene_t *scene,
                               const char *filename,
                               int width, int height)
{
    int success = SLOPE_TRUE;

    cairo_surface_t *surf = cairo_svg_surface_create(
        filename, width, height);

    if (cairo_surface_status(surf) != CAIRO_STATUS_SUCCESS)
        success = SLOPE_FALSE;

    cairo_t *cr = cairo_create(surf);
    slope_rect_t rect;
    slope_rect_set(&rect, 0.0, 0.0, width, height);
    slope_scene_draw(scene, cr, &rect);

    cairo_destroy(cr);
    cairo_surface_destroy(surf);

    return success;
}

int slope_scene_write_to_pdf (slope_scene_t *scene,
                               const char *filename,
                               slope_paper_size_t paper_size,
                               slope_paper_orientation_t orientation)
{
    int success = SLOPE_TRUE;
    int width, height;

    switch (paper_size)
    {
    case SLOPE_A0:
        width = 2384;
        height = 3371;
        break;
    case SLOPE_A1:
        width = 1685;
        height = 2384;
        break;
    case SLOPE_A2:
        width = 1190;
        height = 1684;
        break;
    case SLOPE_A3:
        width = 842;
        height = 1190;
        break;
    case SLOPE_A4:
        width = 595;
        height = 842;
        break;
    case SLOPE_LETTER:
        width = 612;
        height = 792;
        break;
    case SLOPE_B4:
        width = 420;
        height = 595;
        break;
    case SLOPE_B5:
        width = 516;
        height = 729;
        break;
    }
    if (orientation == SLOPE_LANDSCAPE)
    {
        int tmp = width;
        width = height;
        height = tmp;
    }

    cairo_surface_t *surf = cairo_pdf_surface_create(
        filename, width, height);

    if (cairo_surface_status(surf) != CAIRO_STATUS_SUCCESS)
        success = SLOPE_FALSE;

    cairo_t *cr = cairo_create(surf);
    slope_rect_t rect;
    slope_rect_set(&rect, 0.0, 0.0, width, height);
    slope_scene_draw(scene, cr, &rect);

    cairo_destroy(cr);
    cairo_surface_destroy(surf);

    return success;
}


void slope_scene_set_change_callback (slope_scene_t *scene,
                                      slope_callback_t callback)
{
    if (scene == NULL) {
        return;
    }
    scene->change_callback = callback;
}


void slope_scene_notify_appearence_change (slope_scene_t *scene,
                                           slope_data_t *data)
{
    (void) data;
    if (scene == NULL) {
        return;
    }
    if (scene->change_callback) {
        (*scene->change_callback)(scene);
    }
}


void slope_scene_notify_data_change (slope_scene_t *scene,
                                     slope_data_t *data)
{
    if (scene == NULL) {
        return;
    }
    slope_metrics_t *metrics = slope_data_get_metrics(data);
    slope_metrics_update(metrics);
    if (scene->change_callback) {
        (*scene->change_callback)(scene);
    }
}

/* slope/scene.h */
