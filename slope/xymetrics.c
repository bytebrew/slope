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
#include "slope/xyitem_p.h"
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
    metrics->item_list = NULL;
    metrics->figure = NULL;

    self->x_low_bound = self->x_up_bound = 80.0;
    self->y_low_bound = self->y_up_bound = 45.0;

    self->axis_list = NULL;
    slope_item_t *axis = slope_xyaxis_create(
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
        slope_item_t *axis = (slope_item_t*)
            slope_iterator_item(axis_iter);
        slope_item_destroy(axis);
        slope_iterator_next(&axis_iter);
    }
}


void __slope_xymetrics_draw (slope_metrics_t *metrics, cairo_t *cr,
                             const slope_rect_t *rect)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    self->xmin_figure = rect->x + self->x_low_bound;
    self->ymin_figure = rect->y + self->y_low_bound;
    self->xmax_figure = rect->x + rect->width - self->x_up_bound;
    self->ymax_figure = rect->y + rect->height - self->y_up_bound;
    self->width_figure = self->xmax_figure - self->xmin_figure;
    self->height_figure = self->ymax_figure - self->ymin_figure;

    cairo_rectangle(
        cr, self->xmin_figure, self->ymin_figure,
        self->width_figure, self->height_figure);
    cairo_save(cr);
    cairo_clip(cr);

    /* draw user item */
    slope_iterator_t *item_iter =
        slope_list_first(metrics->item_list);
    while (item_iter) {
        slope_item_t *item = (slope_item_t*)
            slope_iterator_item(item_iter);
        if (slope_item_get_visible(item)) {
            __slope_item_draw(item, cr, metrics);
        }
        slope_iterator_next(&item_iter);
    }
    cairo_restore(cr);

    /* draw axis */
    slope_iterator_t *axis_iter =
    slope_list_first(self->axis_list);
    while (axis_iter) {
        slope_item_t *axis = (slope_item_t*)
            slope_iterator_item(axis_iter);
        if (slope_item_get_visible(axis)) {
            __slope_item_draw(axis, cr, metrics);
        }
        slope_iterator_next(&axis_iter);
    }
}


void __slope_xymetrics_update (slope_metrics_t *metrics)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    if (metrics->item_list == NULL) {
        self->xmin = 0.0;
        self->xmax = 1.0;
        self->ymin = 0.0;
        self->ymax = 1.0;
        self->width = self->xmax - self->xmin;
        self->height = self->ymax - self->ymin;
        return;
    }

    slope_iterator_t *iter = slope_list_first(metrics->item_list);
    slope_xyitem_t *item = (slope_xyitem_t*) slope_iterator_item(iter);

    while (item->rescalable == SLOPE_FALSE) {
        slope_iterator_next(&iter);
        item = (slope_xyitem_t*) slope_iterator_item(iter);
    }

    self->xmin = item->xmin;
    self->xmax = item->xmax;
    self->ymin = item->ymin;
    self->ymax = item->ymax;

    slope_iterator_next(&iter);
    while (iter) {
        item = (slope_xyitem_t*) slope_iterator_item(iter);
        if (item->rescalable == SLOPE_FALSE) {
            slope_iterator_next(&iter);
            continue;
        }
        if (item->xmin < self->xmin) self->xmin = item->xmin;
        if (item->xmax > self->xmax) self->xmax = item->xmax;
        if (item->ymin < self->ymin) self->ymin = item->ymin;
        if (item->ymax > self->ymax) self->ymax = item->ymax;
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
    return self->xmin_figure + tmp*self->width_figure;
}


double slope_xymetrics_map_y (const slope_metrics_t *metrics, double y)
{
    const slope_xymetrics_t *self = (const slope_xymetrics_t*) metrics;
    double tmp = (y - self->ymin) /self->height;
    return self->ymax_figure - tmp*self->height_figure;
}


slope_item_t* slope_xymetrics_get_axis (slope_metrics_t *metrics,
                                        slope_xyaxis_type_t type)
{
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;

    slope_iterator_t *axis_iter =
        slope_list_first(self->axis_list);
    while (axis_iter) {
        slope_item_t *axis = (slope_item_t*)
            slope_iterator_item(axis_iter);
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


void slope_xymetrics_set_x_range (slope_metrics_t *metrics,
                                  double xi, double xf)
{
    if (metrics == NULL) {
        return;
    }
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    self->xmin = xi;
    self->xmax = xf;
    self->width = self->xmax - self->xmin;
}


void slope_xymetrics_set_y_range (slope_metrics_t *metrics,
                                  double yi, double yf)
{
    if (metrics == NULL) {
        return;
    }
    slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
    self->ymin = yi;
    self->ymax = yf;
    self->height = self->ymax - self->ymin;
}

/* slope/xymetrics.h */
