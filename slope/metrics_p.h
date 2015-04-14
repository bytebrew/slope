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

#ifndef __SLOPE_METRICS_P_H
#define __SLOPE_METRICS_P_H

#include "slope/metrics.h"

__SLOPE_BEGIN_DECLS

/**
 */
typedef struct _slope_metrics_class slope_metrics_class_t;

/**
 */
struct _slope_metrics_class
{
    void (*destroy_fn) (slope_metrics_t*);
    void (*update_fn) (slope_metrics_t*);
    void (*draw_fn) (slope_metrics_t*, cairo_t*, const slope_rect_t*);
};

/**
 */
struct _slope_metrics
{
    slope_metrics_class_t *klass;
    slope_scene_t *scene;
    int visible;
    slope_list_t *data_list;
};

/**
 */
slope_metrics_class_t* __slope_metrics_get_class();

/**
 */
void __slope_metrics_draw (slope_metrics_t *metrics, cairo_t *cr,
                           const slope_rect_t *rect);

__SLOPE_END_DECLS

#endif /*__SLOPE_METRICS_P_H */
