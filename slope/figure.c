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

#include "slope/figure_p.h"
#include "slope/metrics_p.h"
#include "slope/xymetrics.h"
#include "slope/legend_p.h"
#include "slope/item.h"
#include "slope/list.h"
#include <stdlib.h>
#include <cairo.h>
#include <cairo-svg.h>
#include <cairo-pdf.h>
#include <cairo-ps.h>


slope_figure_t*
slope_figure_create()
{
  slope_figure_t *self = malloc(sizeof(slope_figure_t));
  self->metrics = NULL;
  self->default_metrics = NULL;
  self->change_callback = NULL;
  self->legend = slope_legend_create();
  slope_color_set_name(&self->back_color, SLOPE_WHITE);
  self->fill_back = SLOPE_TRUE;
  self->default_font = slope_font_create("Sans", 10);
  return self;
}


void
slope_figure_destroy (slope_figure_t *self)
{
  if (self == NULL) return;
  slope_item_destroy(self->legend);
  slope_list_destroy(self->metrics);
  slope_font_destroy(self->default_font);
  free(self);
}


void
slope_figure_add_metrics (slope_figure_t *self,
                          slope_metrics_t *metrics)
{
  if (self == NULL) return;
  if (metrics == NULL) return;

  metrics->figure = self;
  self->metrics = slope_list_append(self->metrics, metrics);
  self->default_metrics = metrics;

  if (self->change_callback) {
    self->change_callback(self);
  }
}


slope_list_t*
slope_figure_get_metrics_list (const slope_figure_t *self)
{
  if (self == NULL) return NULL;
  return self->metrics;
}


void
slope_figure_draw (slope_figure_t *self, cairo_t *cr,
                   const slope_rect_t *rect)
{
  slope_iterator_t *met_iter;

  /* perform any pending drawing and clip to the figure's
     rectangle */
  cairo_stroke(cr);
  cairo_save(cr);
  slope_cairo_rectangle(cr, rect);
  cairo_clip(cr);

  /* fill background if required */
  if (self->fill_back) {
    slope_cairo_set_color(cr, &self->back_color);
    cairo_paint(cr);
  }

  /* draw main items */
  SLOPE_LIST_FOREACH (met_iter, self->metrics) {
    slope_metrics_t *met;
    met = (slope_metrics_t*) slope_iterator_data(met_iter);
    if (slope_metrics_get_visible(met)) {
      _slope_metrics_draw(met, cr, rect);
    }
  }

  /* draw legend */
  if (slope_item_get_visible(self->legend)
      && self->default_metrics != NULL)
  {
    _slope_legend_draw(self->legend, cr, self->default_metrics);
  }
  cairo_restore(cr);
}


int
slope_figure_write_to_png (slope_figure_t *figure,
                           const char *filename,
                           int width, int height)
{
  cairo_surface_t *surf;
  cairo_t *cr;
  slope_rect_t rect;
  
  surf = cairo_image_surface_create(
      CAIRO_FORMAT_ARGB32, width, height);
  if (cairo_surface_status(surf) != CAIRO_STATUS_SUCCESS) {
    return SLOPE_ERROR;
  }
  
  cr = cairo_create(surf);
  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(surf);
    return SLOPE_ERROR;
  }
  
  slope_rect_set(&rect, 0.0, 0.0, width, height);
  slope_figure_draw(figure, cr, &rect);
  cairo_surface_write_to_png(surf, filename);
  
  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(surf);
    cairo_destroy(cr);
    return SLOPE_ERROR;
  }
  
  cairo_surface_destroy(surf);
  cairo_destroy(cr);
  return SLOPE_SUCCESS;
}


int
slope_figure_write_to_svg (slope_figure_t *figure,
                           const char *filename,
                           int width, int height)
{
  cairo_surface_t *surf;
  cairo_t *cr;
  slope_rect_t rect;
  
  surf = cairo_svg_surface_create(
      filename, width, height);
  if (cairo_surface_status(surf) != CAIRO_STATUS_SUCCESS) {
    return SLOPE_ERROR;
  }
  
  cr = cairo_create(surf);
  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(surf);
    return SLOPE_ERROR;
  }
  
  slope_rect_set(&rect, 0.0, 0.0, width, height);
  slope_figure_draw(figure, cr, &rect);
  
  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(surf);
    cairo_destroy(cr);
    return SLOPE_ERROR;
  }
  
  cairo_surface_destroy(surf);
  cairo_destroy(cr);
  return SLOPE_SUCCESS;
}


int
slope_figure_write_to_pdf (slope_figure_t *figure,
                           const char *filename,
                           int width, int height)
{
  cairo_surface_t *surf;
  cairo_t *cr;
  slope_rect_t rect;
  
  surf = cairo_pdf_surface_create(
      filename, width, height);
  if (cairo_surface_status(surf) != CAIRO_STATUS_SUCCESS) {
    return SLOPE_ERROR;
  }
  
  cr = cairo_create(surf);
  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(surf);
    return SLOPE_ERROR;
  }
  
  slope_rect_set(&rect, 0.0, 0.0, width, height);
  slope_figure_draw(figure, cr, &rect);

  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(surf);
    cairo_destroy(cr);
    return SLOPE_ERROR;
  }
  
  cairo_surface_destroy(surf);
  cairo_destroy(cr);
  return SLOPE_SUCCESS;
}


int
slope_figure_write_to_ps (slope_figure_t *figure,
                          const char *filename,
                          int width, int height)
{
  cairo_surface_t *surf;
  cairo_t *cr;
  slope_rect_t rect;
  
  surf = cairo_ps_surface_create(
      filename, width, height);
  if (cairo_surface_status(surf) != CAIRO_STATUS_SUCCESS) {
    return SLOPE_ERROR;
  }

  cr = cairo_create(surf);
  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(surf);
    return SLOPE_ERROR;
  }
  
  slope_rect_set(&rect, 0.0, 0.0, width, height);
  slope_figure_draw(figure, cr, &rect);

  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(surf);
    cairo_destroy(cr);
    return SLOPE_ERROR;
  }
  
  cairo_surface_destroy(surf);
  cairo_destroy(cr);
  return SLOPE_SUCCESS;
}


slope_metrics_t*
slope_figure_get_default_metrics (slope_figure_t *figure)
{
  if (figure == NULL) return NULL;
  return figure->default_metrics;
}


void
slope_figure_set_change_callback (slope_figure_t *self,
                                  slope_callback_t callback)
{
  if (self == NULL) return;
  self->change_callback = callback;
}


void
slope_figure_notify_appearence_change (slope_figure_t *self,
                                       slope_item_t *item)
{
  if (self == NULL) return;
  (void) item; /* reserved for possible future use */

  if (self->change_callback) {
    self->change_callback(self);
  }
}


void
slope_figure_notify_data_change (slope_figure_t *self,
                                 slope_item_t *item)
{
  if (self == NULL) return;

  slope_metrics_t *metrics = slope_item_get_metrics(item);
  slope_metrics_update(metrics);
  if (self->change_callback) {
    self->change_callback(self);
  }
}


void
slope_figure_track_region (slope_figure_t *figure,
                           double x1, double y1,
                           double x2, double y2)
{
  if (figure == NULL) return;

  if (x2 < x1) {
    double tmp = x2;
    x2 = x1;
    x1 = tmp;
  }
  if (y2 > y1) {
    double tmp = y2;
    y2 = y1;
    y1 = tmp;
  }

  slope_iterator_t *metr_iter =
    slope_list_first(figure->metrics);
  while (metr_iter) {
    slope_metrics_t *metrics =
      slope_iterator_data(metr_iter);

    /* cartesian coordinates (xymetrics) */
    if (slope_metrics_get_type(metrics) == SLOPE_XYMETRICS) {
      slope_xymetrics_set_x_range(metrics,
          slope_xymetrics_unmap_x(metrics, x1),
          slope_xymetrics_unmap_x(metrics, x2));
      slope_xymetrics_set_y_range(metrics,
          slope_xymetrics_unmap_y(metrics, y1),
          slope_xymetrics_unmap_y(metrics, y2));
    }

    slope_iterator_next(&metr_iter);
  }
}


void
slope_figure_update (slope_figure_t *figure)
{
  if (figure == NULL) return;

  slope_iterator_t *metr_iter =
    slope_list_first(figure->metrics);
  while (metr_iter) {
    slope_metrics_t *metrics =
      slope_iterator_data(metr_iter);
    if (slope_metrics_get_visible(metrics)) {
      slope_metrics_update(metrics);
    }
    slope_iterator_next(&metr_iter);
  }
}


slope_font_t*
slope_figure_get_default_font (slope_figure_t *figure)
{
  if (figure == NULL) return NULL;
  return figure->default_font;
}

/* slope/figure.h */
