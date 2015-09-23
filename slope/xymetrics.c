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
#include "slope/funcplot_p.h"
#include "slope/list.h"
#include <cairo.h>
#include <stdlib.h>


slope_metrics_class_t*
_slope_xymetrics_get_class()
{
  static int first_call = SLOPE_TRUE;
  static slope_metrics_class_t klass;

  if (first_call) {
    ((slope_object_class_t*) &klass)->destroy_fn = _slope_xymetrics_destroy;
    klass.update_fn = _slope_xymetrics_update;
    klass.draw_fn = _slope_xymetrics_draw;
    klass.get_data_rect_fn = _slope_xymetrics_get_data_rect;
    first_call = SLOPE_FALSE;
  }

  return &klass;
}


slope_metrics_t*
slope_xymetrics_create()
{
  slope_xymetrics_t *self = malloc(sizeof(slope_xymetrics_t));
  slope_metrics_t *metrics = (slope_metrics_t*) self;
  slope_xymetrics_private_t *priv = malloc(sizeof(slope_xymetrics_private_t));
  slope_metrics_private_t *metrics_priv = (slope_metrics_private_t*) priv;

  ((slope_object_t*) self)->klass = (slope_object_class_t*) _slope_xymetrics_get_class();
  ((slope_object_t*) self)->priv = (slope_object_private_t*) priv;
    
  metrics_priv->type = SLOPE_XYMETRICS;
  metrics_priv->visible = SLOPE_TRUE;
  metrics_priv->item_list = NULL;
  metrics_priv->figure = NULL;

  metrics_priv->x_low_bound = 85.0;
  metrics_priv->y_low_bound = 55.0;
  metrics_priv->x_up_bound = 85.0;
  metrics_priv->y_up_bound = 55.0;

  priv->axis_list = NULL;
  slope_item_t *axis = slope_xyaxis_create(
      metrics, SLOPE_XYAXIS_TOP, "");
  priv->axis_list = slope_list_append(priv->axis_list, axis);
  axis = slope_xyaxis_create(
      metrics, SLOPE_XYAXIS_BOTTOM, "X");
  priv->axis_list = slope_list_append(priv->axis_list, axis);
  axis = slope_xyaxis_create(
      metrics, SLOPE_XYAXIS_LEFT, "Y");
  priv->axis_list = slope_list_append(priv->axis_list, axis);
  axis = slope_xyaxis_create(
      metrics, SLOPE_XYAXIS_RIGHT, "");
  priv->axis_list = slope_list_append(priv->axis_list, axis);

  slope_metrics_update(metrics);
  return metrics;
}


void
_slope_xymetrics_destroy (slope_object_t *metrics)
{
  slope_xymetrics_private_t *priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
  slope_iterator_t *axis_iter;

  /* destroy axis */
  SLOPE_LIST_FOREACH (axis_iter, priv->axis_list) {
    slope_item_t *axis = (slope_item_t*) slope_iterator_data(axis_iter);
    slope_item_destroy(axis);
  }
  slope_list_destroy(priv->axis_list);
  free(priv);
}


void
_slope_xymetrics_draw (slope_metrics_t *metrics, cairo_t *cr,
                       const slope_rect_t *rect)
{
  slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
  slope_xymetrics_private_t *priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
  slope_metrics_private_t *metrics_priv = (slope_metrics_private_t*) priv;
  slope_iterator_t *iter;

  metrics_priv->xmin_figure = rect->x + metrics_priv->x_low_bound;
  metrics_priv->ymin_figure = rect->y + metrics_priv->y_low_bound;
  metrics_priv->xmax_figure = rect->x + rect->width - metrics_priv->x_up_bound;
  metrics_priv->ymax_figure = rect->y + rect->height - metrics_priv->y_up_bound;
  metrics_priv->width_figure = metrics_priv->xmax_figure - metrics_priv->xmin_figure;
  metrics_priv->height_figure = metrics_priv->ymax_figure - metrics_priv->ymin_figure;

  cairo_save(cr);
  cairo_new_path(cr);
  cairo_rectangle(
      cr, metrics_priv->xmin_figure, metrics_priv->ymin_figure,
      metrics_priv->width_figure, metrics_priv->height_figure);
  cairo_clip(cr);

  /* draw user item */
  SLOPE_LIST_FOREACH (iter, metrics_priv->item_list) {
    slope_item_t *item = (slope_item_t*) slope_iterator_data(iter);
    if (slope_item_get_visible(item)) {
      _slope_item_draw(item, cr, metrics);
    }
  }
  cairo_restore(cr);

  /* draw axis */
  SLOPE_LIST_FOREACH (iter, priv->axis_list) {
    slope_item_t *axis = (slope_item_t*) slope_iterator_data(iter);
    if (slope_item_get_visible(axis)) {
      _slope_item_draw(axis, cr, metrics);
    }
  }
}


void
_slope_xymetrics_update (slope_metrics_t *metrics)
{
  slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
  slope_xymetrics_private_t *priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
  slope_metrics_private_t *metrics_priv = (slope_metrics_private_t*) priv;
  slope_iterator_t *iterator = NULL;
  slope_item_t *item = NULL;
  slope_rect_t rect;

  if (metrics_priv->item_list == NULL) {
    priv->xmin = 0.0;
    priv->xmax = 1.0;
    priv->ymin = 0.0;
    priv->ymax = 1.0;
    priv->width = priv->xmax - priv->xmin;
    priv->height = priv->ymax - priv->ymin;
    return;
  }

  iterator = slope_list_first(metrics_priv->item_list);
  item = (slope_item_t*) slope_iterator_data(iterator);

  while (slope_item_get_rescalable(item) == SLOPE_FALSE) {
    slope_iterator_next(&iterator);
    item = (slope_item_t*) slope_iterator_data(iterator);
  }

  slope_item_get_rect(item, &rect);

  priv->xmin = rect.x;
  priv->xmax = rect.x + rect.width;
  priv->ymin = rect.y;
  priv->ymax = rect.y + rect.height;

  slope_iterator_next(&iterator);

  while (iterator) {
    item = (slope_item_t*) slope_iterator_data(iterator);

    if (slope_item_get_rescalable(item) == SLOPE_FALSE) {
      slope_iterator_next(&iterator);
      continue;
    }

    slope_item_get_rect(item, &rect);

    if (rect.x               < priv->xmin) priv->xmin = rect.x;
    if (rect.x + rect.width  > priv->xmax) priv->xmax = rect.x + rect.width;
    if (rect.y               < priv->ymin) priv->ymin = rect.y;
    if (rect.y + rect.height < priv->ymax) priv->ymax = rect.y + rect.height;

    slope_iterator_next(&iterator);
  }

  double xbound = (priv->xmax - priv->xmin) /20.0;
  priv->xmin -= xbound;
  priv->xmax += xbound;
  double ybound = (priv->ymax - priv->ymin) /20.0;
  priv->ymin -= ybound;
  priv->ymax += ybound;
  priv->width = priv->xmax - priv->xmin;
  priv->height = priv->ymax - priv->ymin;
}


double
slope_xymetrics_map_x (const slope_metrics_t *metrics, double x)
{
    slope_xymetrics_private_t *priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
    slope_metrics_private_t *metrics_priv = (slope_metrics_private_t*) priv;
    
  double tmp = (x - priv->xmin) /priv->width;
  return metrics_priv->xmin_figure + tmp*metrics_priv->width_figure;
}


double
slope_xymetrics_map_y (const slope_metrics_t *metrics, double y)
{
    slope_xymetrics_private_t *priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
    slope_metrics_private_t *metrics_priv = (slope_metrics_private_t*) priv;
    
  double tmp = (y - priv->ymin) /priv->height;
  return metrics_priv->ymax_figure - tmp*metrics_priv->height_figure;
}


double
slope_xymetrics_unmap_x (const slope_metrics_t *metrics, double x)
{
    slope_xymetrics_private_t *priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
    slope_metrics_private_t *metrics_priv = (slope_metrics_private_t*) priv;
    
  double tmp = (x - metrics_priv->xmin_figure) /metrics_priv->width_figure;
  return priv->xmin + tmp*priv->width;
}


double
slope_xymetrics_unmap_y (const slope_metrics_t *metrics, double y)
{
    slope_xymetrics_private_t *priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
    slope_metrics_private_t *metrics_priv = (slope_metrics_private_t*) priv;
    
  double tmp = (metrics_priv->ymax_figure - y) /metrics_priv->height_figure;
  return priv->ymin + tmp*priv->height;
}


slope_item_t*
slope_xymetrics_get_axis (slope_metrics_t *metrics,
                          slope_xyaxis_type_t type)
{
    slope_xymetrics_private_t *priv;
  slope_iterator_t *axis_iter;
  
  if (metrics == NULL) {
      return NULL;
  }
  
  priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);

  SLOPE_LIST_FOREACH (axis_iter, priv->axis_list) {
    slope_item_t *axis = (slope_item_t*) slope_iterator_data(axis_iter);
    if (slope_xyaxis_get_type(axis) == type) {
      return axis;
    }
  }
  return NULL;
}


void
slope_xymetrics_set_x_boundary (slope_metrics_t *metrics,
                                double low, double hi)
{
    slope_xymetrics_private_t *priv;
    slope_metrics_private_t *metrics_priv;
    
  if (metrics == NULL) {
    return;
  }
  
  priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
  metrics_priv = (slope_metrics_private_t*) priv;
  
  metrics_priv->x_low_bound = low;
  metrics_priv->x_up_bound = hi;
}


void
slope_xymetrics_set_y_boundary (slope_metrics_t *metrics,
                                double low, double hi)
{
    slope_xymetrics_private_t *priv;
    slope_metrics_private_t *metrics_priv;
    
    if (metrics == NULL) {
        return;
    }
    
    priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
    metrics_priv = (slope_metrics_private_t*) priv;
    
  metrics_priv->y_low_bound = low;
  metrics_priv->y_up_bound = hi;
}


void
slope_xymetrics_set_x_range (slope_metrics_t *metrics,
                             double xi, double xf)
{
    slope_xymetrics_private_t *priv;
    
    if (metrics == NULL) {
        return;
    }
    
    priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
    
  priv->xmin = xi;
  priv->xmax = xf;
  priv->width = priv->xmax - priv->xmin;
}


void
slope_xymetrics_set_y_range (slope_metrics_t *metrics,
                             double yi, double yf)
{
    slope_xymetrics_private_t *priv;
    
    if (metrics == NULL) {
        return;
    }
    
    priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
    
  priv->ymin = yi;
  priv->ymax = yf;
  priv->height = priv->ymax - priv->ymin;
}


void
_slope_xymetrics_get_data_rect (const slope_metrics_t *metrics,
                                slope_rect_t *rect)
{
    slope_xymetrics_private_t *priv;
    
    priv = SLOPE_XYMETRICS_GET_PRIVATE(metrics);
    rect->x = priv->xmin;
    rect->y = priv->ymin;
    rect->width = priv->width;
    rect->height = priv->height;
}

/* slope/xymetrics.h */
