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

#include "slope-scene.h"
#include "slope-scene_p.h"
#include "slope-plotable.h"
#include <stdlib.h>
#include <cairo.h>


/**
 * Creates a new scene
 */
slope_scene_t* slope_scene_create ()
{
    slope_scene_t *scene = malloc(sizeof(slope_scene_t));
    scene->plotables = NULL;
    scene->fill_back = 1;
    slope_color_set_by_name(&scene->back_color, SLOPE_WHITE);
    scene->x_low_b = scene->x_up_b = 0.0;
    scene->y_low_b = scene->y_up_b = 0.0;
    return scene;
}

/**
 * Drestroys scene
 */
void slope_scene_destroy (slope_scene_t *scene)
{
    slope_list_destroy(scene->plotables);
    free(scene);
    scene = NULL;
}

/**
 * Draw the contents of scenes to the surface targeted by cr
 */
void slope_scene_draw (slope_scene_t *scene, cairo_t *cr, slope_rect_t *area)
{
    slope_rect_t scene_rect;
    slope_rect_set(&scene_rect,
                   area->x + scene->x_low_b,
                   area->y + scene->y_low_b,
                   area->width - scene->x_low_b - scene->x_up_b,
                   area->height - scene->y_low_b - scene->y_up_b);

    /* paint the background if required */
    cairo_save(cr);
    cairo_stroke(cr);
    if (scene->fill_back) {
        cairo_set_source_rgba(cr,
                              scene->back_color.red,
                              scene->back_color.green,
                              scene->back_color.blue,
                              scene->back_color.alpha);
        cairo_paint(cr);
    }
    cairo_restore(cr);
    
    /* draw contents */
    cairo_save(cr);
    cairo_rectangle(cr, scene_rect.x, scene_rect.y,
                    scene_rect.width, scene_rect.height);
    cairo_clip(cr);
    slope_iterator_t *iter =
        slope_list_first(scene->plotables);
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
 * Writes the scene to a png file
 */
void slope_scene_write_to_png (slope_scene_t *scene,
                               const char *filename,
                               int width, int height)
{
    cairo_surface_t *image =
        cairo_image_surface_create(CAIRO_FORMAT_RGB24,
                                   width, height);
    cairo_t *cr = cairo_create(image);
    slope_rect_t rect;
    slope_rect_set(&rect, 0.0, 0.0, width, height);
    slope_scene_draw(scene, cr, &rect);
    cairo_surface_write_to_png(image, filename);
    cairo_surface_destroy(image);
    cairo_destroy(cr);
}


void slope_scene_set_back_color_by_name (slope_scene_t *scene,
                                         slope_color_name_t color_name)
{
    slope_color_set_by_name(&scene->back_color, color_name);
}


void slope_scene_add_plotable (slope_scene_t *scene,
                               slope_plotable_t *plot)
{
    scene->plotables = slope_list_append(scene->plotables, plot);
}


/* slope-scene.c */