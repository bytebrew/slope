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

#ifndef __SLOPE_SCENE_H
#define __SLOPE_SCENE_H

#include "slope/list.h"
#include "slope/primitives.h"

__SLOPE_BEGIN_DECLS

/**
 */
slope_public slope_scene_t* slope_scene_create();

/**
 */
slope_public slope_list_t*
slope_scene_get_metrics_list (slope_scene_t *scene);

/**
 */
slope_public void
slope_scene_add_metrics (slope_scene_t *scene,
                         slope_metrics_t *metrics);

/**
 */
slope_public void
slope_scene_destroy (slope_scene_t *scene);

/**
 */
slope_public void
slope_scene_draw (slope_scene_t *scene, cairo_t *cr,
                  const slope_rect_t *rect);

/**
 */
slope_public int
slope_scene_write_to_png (slope_scene_t *scene,
                          const char *filename,
                          int width, int height);


/**
 */
slope_public int
slope_scene_write_to_svg(slope_scene_t *scene,
                          const char *filename,
                          int width, int height);


/**
 */
slope_public int
slope_scene_write_to_pdf(slope_scene_t *scene,
                          const char *filename,
                          slope_paper_size_t paper_size,
                          slope_paper_orientation_t orientation);

/**
 */
slope_public void
slope_scene_set_change_callback (slope_scene_t *scene,
                                 slope_callback_t callback);

/**
 */
slope_public void
slope_scene_notify_appearence_change (slope_scene_t *scene,
                                      slope_data_t *data);

/**
 */
slope_public void
slope_scene_notify_data_change (slope_scene_t *scene,
                                slope_data_t *data);

__SLOPE_END_DECLS

#endif /*__SLOPE_SCENE_H */
