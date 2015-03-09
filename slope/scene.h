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

#ifndef _SLOPE_SCENE_H_
#define _SLOPE_SCENE_H_

#include "slope/primitives.h"
#include "slope/metrics.h"
#include "slope/list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _slope_scene slope_scene_t;

/**
 * Drestroys scene
 * @param scene the scene to be destroyed
 */
void slope_scene_destroy (slope_scene_t *scene);

/**
 * Draw the contents of scenes to the surface targeted by cr
 * @param scene the scene to be drawn
 * @param cr the cairo context to draw the scene
 * @param area the target rectangle int surface coordinates
 */
void slope_scene_draw (slope_scene_t *scene, cairo_t *cr,
                       slope_rect_t *area);

/**
 * Writes the scene to a png file
 * @param scene the scene to be writen
 * @param file name location for the output file
 * @param width width of the output image
 * @param height height of the output image
 */
void slope_scene_write_to_png (slope_scene_t *scene,
                               const char *filename,
                               int width, int height);

void slope_scene_set_back_color_name (slope_scene_t *scene,
                                      slope_color_name_t color);

void slope_scene_add_metrics (slope_scene_t *scene,
                              slope_metrics_t *plot);

slope_list_t* slope_scene_metrics_list (slope_scene_t *scene);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_SCENE_H_*/