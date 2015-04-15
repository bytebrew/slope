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

#include "slope/xymetrics_p.h"
#include "slope/xydata_p.h"
#include "slope/list.h"
#include <cairo.h>
#include <stdlib.h>


slope_metrics_class_t* __slope_xymetrics_get_class()
{
    static int first_call = SLOPE_TRUE;
    static slope_metrics_class_t klass;

    if (first_call) {
        klass.destroy_fn = __slope_xymetrics_destroy;
        klass.update_fn = __slope_xymetrics_update;
        klass.draw_fn = __slope_xymetrics_draw;
        first_call = SLOPE_FALSE;
    }

    return &klass;
}


slope_metrics_t* slope_xymetrics_create()
{
    slope_xymetrics_t *self = malloc(sizeof(slope_xymetrics_t));
    slope_metrics_t *metrics = (slope_metrics_t*) self;

    metrics->klass = __slope_xymetrics_get_class();
    metrics->visible = SLOPE_TRUE;
    metrics->data_list = NULL;
    metrics->scene = NULL;

    self->x_low_bound = self->x_up_bound = 80.0;
    self->y_low_bound = self->y_up_bound = 45.0;

    self->axis_list = NULL;
    slope_data_t *axis = slope_xyaxis_create(
        metrics, SLOPE_XYAXIS_TOP, "");
    self->axis_list = slope_list_append(self->axis_list, axis);
    axis = slope_xyaxis_create(
        metrics, SLOPE_XYAXIS_BOTTOM, "X");
    self->axis_list = slope_list_append(self->axis_list, axis);
    axis = slope_xyaxis_create(
        metrics, SLOPE_XYAXIS_LEFT, "Y");
    self->axis_list = slope_list_append(self->axis_list, axis);
    axis = slope_xyaxis_create(
        metrics, SLOPE_XYAXIS_RIGHT, "");
    self->axis_list = slope_list_append(self->axis_list, axis);

    slope_metrics_update(metrics);
    return metrics;
}


void __slope_xymetrics_destroy (slope_metrics_t *metrics)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;

    /* destroy axis */
    slope_iterator_t *axis_iter =
    slope_list_first(self->axis_list);
    while (axis_iter) {
        slope_data_t *axis = (slope_data_t*)
            slope_iterator_data(axis_iter);
        slope_data_destroy(axis);
        slope_iterator_next(&axis_iter);
    }
}


void __slope_xymetrics_draw (slope_metrics_t *metrics, cairo_t *cr,
                             const slope_rect_t *rect)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    self->xmin_scene = rect->x + self->x_low_bound;
    self->ymin_scene = rect->y + self->y_low_bound;
    self->xmax_scene = rect->x + rect->width - self->x_up_bound;
    self->ymax_scene = rect->y + rect->height - self->y_up_bound;
    self->width_scene = self->xmax_scene - self->xmin_scene;
    self->height_scene = self->ymax_scene - self->ymin_scene;

    cairo_rectangle(
        cr, self->xmin_scene, self->ymin_scene,
        self->width_scene, self->height_scene);
    cairo_save(cr);
    cairo_clip(cr);

    /* draw user data */
    slope_iterator_t *data_iter =
        slope_list_first(metrics->data_list);
    while (data_iter) {
        slope_data_t *data = (slope_data_t*)
            slope_iterator_data(data_iter);
        if (slope_data_get_visible(data)) {
            __slope_data_draw(data, cr, metrics);
        }
        slope_iterator_next(&data_iter);
    }
    cairo_restore(cr);

    /* draw axis */
    slope_iterator_t *axis_iter =
    slope_list_first(self->axis_list);
    while (axis_iter) {
        slope_data_t *axis = (slope_data_t*)
            slope_iterator_data(axis_iter);
        if (slope_data_get_visible(axis)) {
            __slope_data_draw(axis, cr, metrics);
        }
        slope_iterator_next(&axis_iter);
    }
}


void __slope_xymetrics_update (slope_metrics_t *metrics)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    if (metrics->data_list == NULL) {
        self->xmin = 0.0;
        self->xmax = 1.0;
        self->ymin = 0.0;
        self->ymax = 1.0;
        self->width = self->xmax - self->xmin;
        self->height = self->ymax - self->ymin;
        return;
    }

    slope_iterator_t *iter = slope_list_first(metrics->data_list);
    slope_xydata_t *data = (slope_xydata_t*) slope_iterator_data(iter);

    while (data->rescalable == SLOPE_FALSE) {
        slope_iterator_next(&iter);
        data = (slope_xydata_t*) slope_iterator_data(iter);
    }

    self->xmin = data->xmin;
    self->xmax = data->xmax;
    self->ymin = data->ymin;
    self->ymax = data->ymax;

    slope_iterator_next(&iter);
    while (iter) {
        data = (slope_xydata_t*) slope_iterator_data(iter);
        if (data->rescalable == SLOPE_FALSE) {
            slope_iterator_next(&iter);
            continue;
        }
        if (data->xmin < self->xmin) self->xmin = data->xmin;
        if (data->xmax > self->xmax) self->xmax = data->xmax;
        if (data->ymin < self->ymin) self->ymin = data->ymin;
        if (data->ymax > self->ymax) self->ymax = data->ymax;
        slope_iterator_next(&iter);
    }
    double xbound = (self->xmax - self->xmin) /20.0;
    self->xmin -= xbound;
    self->xmax += xbound;
    double ybound = (self->ymax - self->ymin) /20.0;
    self->ymin -= ybound;
    self->ymax += ybound;
    self->width = self->xmax - self->xmin;
    self->height = self->ymax - self->ymin;
}


double slope_xymetrics_map_x (const slope_metrics_t *metrics, double x)
{
    const slope_xymetrics_t *self = (const slope_xymetrics_t*) metrics;
    double tmp = (x - self->xmin) /self->width;
    return self->xmin_scene + tmp*self->width_scene;
}


double slope_xymetrics_map_y (const slope_metrics_t *metrics, double y)
{
    const slope_xymetrics_t *self = (const slope_xymetrics_t*) metrics;
    double tmp = (y - self->ymin) /self->height;
    return self->ymax_scene - tmp*self->height_scene;
}


slope_data_t* slope_xymetrics_get_axis (slope_metrics_t *metrics,
                                        slope_xyaxis_type_t type)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;

    slope_iterator_t *axis_iter =
        slope_list_first(self->axis_list);
    while (axis_iter) {
        slope_data_t *axis = (slope_data_t*)
            slope_iterator_data(axis_iter);
        if (slope_xyaxis_get_type(axis) == type) {
            return axis;
        }
        slope_iterator_next(&axis_iter);
    }
    return NULL;
}


slope_public void
slope_xymetrics_set_x_boundary (slope_metrics_t *metrics,
                                double low, double hi)
{
    if (metrics == NULL) {
        return;
    }
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    self->x_low_bound = low;
    self->x_up_bound = hi;
}


slope_public void
slope_xymetrics_set_y_boundary (slope_metrics_t *metrics,
                                double low, double hi)
{
    if (metrics == NULL) {
        return;
    }
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    self->y_low_bound = low;
    self->y_up_bound = hi;
}

/* slope/xymetrics.h */
