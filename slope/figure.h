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
slope_public slope_figure_t* slope_figure_create();

/**
 */
slope_public slope_list_t*
slope_figure_get_metrics_list (const slope_figure_t *figure);

/**
 */
slope_public void
slope_figure_add_metrics (slope_figure_t *figure,
                          slope_metrics_t *metrics);

/**
 */
slope_public void
slope_figure_destroy (slope_figure_t *figure);

/**
 */
slope_public void
slope_figure_draw (slope_figure_t *figure, cairo_t *cr,
                   const slope_rect_t *rect);

/**
 */
slope_public void
slope_figure_write_to_png (slope_figure_t *figure,
                          const char *filename,
                          int width, int height);

/**
 */
slope_public int
slope_figure_write_to_svg (slope_figure_t *figure,
                           const char *filename,
                           int width, int height);

/**
 */
slope_public int
slope_figure_write_to_pdf (slope_figure_t *figure,
                           const char *filename,
                           int width, int height);

/**
 */
slope_public int
slope_figure_write_to_ps (slope_figure_t *figure,
                          const char *filename,
                          int width, int height);

/**
 */
slope_public slope_metrics_t*
slope_figure_get_default_metrics (slope_figure_t *figure);

/**
 */
slope_public void
slope_figure_set_change_callback (slope_figure_t *figure,
                                  slope_callback_t callback);

/**
 */
slope_public void
slope_figure_notify_appearence_change (slope_figure_t *figure,
                                       slope_item_t *item);

/**
 */
slope_public void
slope_figure_notify_data_change (slope_figure_t *figure,
                                 slope_item_t *item);

/**
 */
slope_public void
slope_figure_track_region (slope_figure_t *figure,
                           double x1, double y1,
                           double x2, double y2);

/**
 */
slope_public void
slope_figure_update (slope_figure_t *figure);

__SLOPE_END_DECLS

#endif /*__SLOPE_SCENE_H */
