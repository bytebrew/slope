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

#include "slope/metrics_p.h"
#include "slope/item_p.h"
#include "slope/list.h"
#include "slope/figure.h"
#include <cairo.h>
#include <stdlib.h>


void
_slope_metrics_destroy (slope_metrics_t *metrics)
{
    slope_metrics_private_t *priv = SLOPE_METRICS_GET_PRIVATE(metrics);
    slope_list_destroy(priv->item_list);
}


void
slope_metrics_destroy (slope_metrics_t *metrics)
{
    slope_object_destroy((slope_object_t*) metrics);
}


int
slope_metrics_get_visible (const slope_metrics_t *metrics)
{
    slope_metrics_private_t *priv;
    
  if (metrics == NULL) return SLOPE_FALSE;
  
  priv = SLOPE_METRICS_GET_PRIVATE(metrics);
  return priv->visible;
}


slope_metrics_type_t
slope_metrics_get_type (const slope_metrics_t *metrics)
{
    slope_metrics_private_t *priv;
    
  if (metrics == NULL) return SLOPE_METRICS_INVALID;
  
  priv = SLOPE_METRICS_GET_PRIVATE(metrics);
  return priv->type;
}


void
slope_metrics_toggle_visible (slope_metrics_t *metrics,
                              slope_bool_t visible)
{
    slope_metrics_private_t *priv;
    
  if (metrics == NULL) return;
  
  priv = SLOPE_METRICS_GET_PRIVATE(metrics);
  priv->visible = visible;
}


void
slope_metrics_update (slope_metrics_t *metrics)
{
  slope_metrics_class_t *klass;
    
  if (metrics == NULL) return;
  
  klass = SLOPE_METRICS_GET_CLASS(metrics);
  
  if (klass->update_fn) {
    klass->update_fn(metrics);
  }
}


void
_slope_metrics_draw (slope_metrics_t *metrics, cairo_t *cr,
                     const slope_rect_t *rect)
{
    slope_metrics_class_t *klass = SLOPE_METRICS_GET_CLASS(metrics);
  klass->draw_fn(metrics, cr, rect);
}


void
slope_metrics_add_item (slope_metrics_t *metrics,
                        slope_item_t *item)
{
    slope_metrics_private_t *priv;
    
  if (metrics == NULL || item == NULL) {
    return;
  }
  
  priv = SLOPE_METRICS_GET_PRIVATE(metrics);
  
  _slope_item_set_metrics (item, metrics);
  
  priv->item_list = slope_list_append(priv->item_list, item);
  slope_metrics_update(metrics);
  slope_figure_notify_appearence_change(priv->figure, item);
}


void
slope_metrics_remove_item (slope_metrics_t *metrics,
                           slope_item_t *item)
{
    slope_metrics_private_t *priv;
    
  if (metrics == NULL || item == NULL) {
    return;
  }
  
  priv = SLOPE_METRICS_GET_PRIVATE(metrics);
  
  int change = SLOPE_FALSE;
  slope_iterator_t *iter = slope_list_first(priv->item_list);
  while (iter) {
    slope_item_t *curr_item = (slope_item_t*) slope_iterator_data(iter);
    if (curr_item == item) {
      iter = slope_list_remove(priv->item_list, iter);
      _slope_item_set_metrics (item, NULL);
      change = SLOPE_TRUE;
    } else {
      slope_iterator_next(&iter);
    }
  }
  
  if (change) {
    slope_metrics_update(metrics);
    slope_figure_notify_appearence_change(priv->figure, item);
  }
}


slope_list_t*
slope_metrics_get_item_list (const slope_metrics_t *metrics)
{
    slope_metrics_private_t *priv;
    
  if (metrics == NULL) return NULL;
  
  priv = SLOPE_METRICS_GET_PRIVATE(metrics);
  return priv->item_list;
}


slope_figure_t*
slope_metrics_get_figure (const slope_metrics_t *metrics)
{
    slope_metrics_private_t *priv;
    
  if (metrics == NULL) return NULL;
  
  priv = SLOPE_METRICS_GET_PRIVATE(metrics);
  return priv->figure;
}


void
_slope_metrics_set_figure (slope_metrics_t *metrics,
                           slope_figure_t *figure)
{
    slope_metrics_private_t *priv;
    
    if (metrics == NULL) return;
    
    priv = SLOPE_METRICS_GET_PRIVATE(metrics);
    priv->figure = figure;
}


void
slope_metrics_get_data_rect (const slope_metrics_t *metrics,
                             slope_rect_t *rect)
{
    slope_metrics_class_t *klass;
    
    if (metrics == NULL) return;
    
    klass = SLOPE_METRICS_GET_CLASS(metrics);
    klass->get_data_rect (metrics, rect);
}


void
slope_metrics_get_figure_rect (const slope_metrics_t *metrics,
                               slope_rect_t *rect)
{
    slope_metrics_private_t *priv;
    
    if (metrics == NULL) return;
    
    priv = SLOPE_METRICS_GET_PRIVATE(metrics);
    rect->x = priv->xmin_figure;
    rect->y = priv->ymin_figure;
    rect->width = priv->width_figure;
    rect->height = priv->height_figure;
}

/* slope/metrics.h */
