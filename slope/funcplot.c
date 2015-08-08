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

#include "slope/funcplot_p.h"
#include "slope/xymetrics_p.h"
#include "slope/text.h"
#include <stdlib.h>
#include <string.h>


#define CIRC_RAD          3.0
#define CIRC_RAD_SQR      9.0
#define TWO_CIRC_RAD_SQR  36.0

#define TRI_RAD           4.0
#define TRI_RAD_SQR       16.0
#define TWO_TRI_RAD_SQR   64.0

#define PLUS_RAD          5.0
#define PLUS_RAD_SQR      25.0
#define TWO_PLUS_RAD_SQR  100.0


slope_item_class_t*
_slope_funcplot_get_class()
{
  static int first_call = SLOPE_TRUE;
  static slope_item_class_t klass;

  if (first_call) {
    klass.destroy_fn = NULL;
    klass.draw_fn = _slope_funcplot_draw;
    klass.draw_thumb_fn = _slope_funcplot_draw_thumb;
    first_call = SLOPE_FALSE;
  }

  return &klass;
}


void
_slope_funcplot_init (slope_item_t *parent)
{
  slope_funcplot_t *self = (slope_funcplot_t*) parent;
  self->antialias = SLOPE_TRUE;
  self->line_width = 1.0;
  self->fill_symbol = SLOPE_TRUE;
  self->rescalable = SLOPE_TRUE;
  parent->name = NULL;
  parent->visible = SLOPE_TRUE;
  parent->has_thumb = SLOPE_TRUE;
  parent->metrics = NULL;
  parent->font = NULL;
  parent->klass = _slope_funcplot_get_class();
}


slope_item_t*
slope_funcplot_create()
{
  slope_funcplot_t *self = malloc(sizeof(slope_funcplot_t));
  slope_item_t *parent = (slope_item_t*) self;
  _slope_funcplot_init(parent);
  return parent;
}


slope_item_t*
slope_funcplot_create_simple (const double *vx,
                              const double *vy,
                              const int n,
                              const char *name,
                              const char *fmt)
{
  slope_funcplot_t *self = malloc(sizeof(slope_funcplot_t));
  slope_item_t *parent = (slope_item_t*) self;
  _slope_funcplot_init(parent);
  slope_funcplot_set(parent, vx, vy, n, name, fmt);
  return parent;
}


void
slope_funcplot_set (slope_item_t *item,
                    const double *vx, const double *vy,
                    const int n,
                    const char *name,
                    const char *fmt)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;
  self->vx = vx;
  self->vy = vy;
  self->n = n;
  if (item->name) {
    free(item->name);
  }
  item->name = strdup(name);
  slope_color_set_name(&self->color, _slope_item_parse_color(fmt));
  self->scatter = _slope_item_parse_scatter(fmt);
  if (self->scatter == SLOPE_PLUSSES) self->antialias = SLOPE_FALSE;
  _slope_funcplot_check_ranges(item);
  slope_item_notify_data_change(item);
}


void
slope_funcplot_set_item (slope_item_t *item,
                         const double *vx, const double *vy,
                         const int n)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;
  self->vx = vx;
  self->vy = vy;
  self->n = n;
  _slope_funcplot_check_ranges(item);
  slope_item_notify_data_change(item);
}


void
slope_funcplot_update_item (slope_item_t *item,
                            const double *vx, const double *vy,
                            const int n)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;
  self->vx = vx;
  self->vy = vy;
  self->n = n;
  slope_item_notify_appearence_change(item);
}


void
_slope_funcplot_draw (slope_item_t *item, cairo_t *cr,
                      const slope_metrics_t *metrics)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;

  slope_cairo_set_color(cr, &self->color);
  if (self->antialias) {
    cairo_set_antialias(
        cr, CAIRO_ANTIALIAS_SUBPIXEL);
  }
  else {
    cairo_set_antialias(
        cr, CAIRO_ANTIALIAS_NONE);
  }
  cairo_set_line_width(cr,self->line_width);

  switch (self->scatter) {
    case SLOPE_LINE:
      _slope_funcplot_draw_line(item, cr, metrics);
      break;
    case SLOPE_CIRCLES:
      _slope_funcplot_draw_circles(item, cr, metrics);
      break;
    case SLOPE_TRIANGLES:
      _slope_funcplot_draw_triangles(item, cr, metrics);
      break;
    case SLOPE_SQUARES:
      _slope_funcplot_draw_squares(item, cr, metrics);
      break;
    case SLOPE_PLUSSES:
      _slope_funcplot_draw_plusses(item, cr, metrics);
      break;
    case SLOPE_LINE|SLOPE_CIRCLES:
      _slope_funcplot_draw_line_circles(item, cr, metrics);
      break;
    case SLOPE_LINE|SLOPE_TRIANGLES:
      _slope_funcplot_draw_line_triangles(item, cr, metrics);
      break;
    case SLOPE_LINE|SLOPE_PLUSSES:
      _slope_funcplot_draw_line_plusses(item, cr, metrics);
      break;
  }
}


void
_slope_funcplot_draw_line (slope_item_t *item, cairo_t *cr,
                           const slope_metrics_t *metrics)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;

  const double *vx = self->vx;
  const double *vy = self->vy;
  const int n = self->n;

  double x1 = slope_xymetrics_map_x(metrics, vx[0]);
  double y1 = slope_xymetrics_map_y(metrics, vy[0]);
  cairo_move_to(cr, x1, y1);

  int k;
  for (k=1; k<n; k++) {
    double x2 = slope_xymetrics_map_x(metrics, vx[k]);
    double y2 = slope_xymetrics_map_y(metrics, vy[k]);

    double dx = x2 - x1;
    double dy = y2 - y1;
    double distsqr = dx*dx + dy*dy;

    if (distsqr >= TWO_CIRC_RAD_SQR) {
      cairo_line_to(cr, x2, y2);
      x1 = x2;
      y1 = y2;
    }
  }
  cairo_stroke(cr);
}


void
_slope_funcplot_draw_circles (slope_item_t *item, cairo_t *cr,
                              const slope_metrics_t *metrics)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;

  const double *vx = self->vx;
  const double *vy = self->vy;
  const int n = self->n;

  double x1 = slope_xymetrics_map_x(metrics, vx[0]);
  double y1 = slope_xymetrics_map_y(metrics, vy[0]);
  cairo_move_to(cr, x1 + CIRC_RAD, y1);
  cairo_arc(cr, x1, y1, CIRC_RAD, 0.0, 6.283185);
  if (self->fill_symbol) cairo_fill(cr);

  int k;
  for (k=1; k<n; k++) {
    double x2 = slope_xymetrics_map_x(metrics, vx[k]);
    double y2 = slope_xymetrics_map_y(metrics, vy[k]);

    double dx = x2 - x1;
    double dy = y2 - y1;
    double distsqr = dx*dx + dy*dy;

    if (distsqr >= TWO_CIRC_RAD_SQR) {
      cairo_move_to(cr, x2 + CIRC_RAD, y2);
      cairo_arc(cr, x2, y2, CIRC_RAD, 0.0, 6.283185);
      if (self->fill_symbol) cairo_fill(cr);
      x1 = x2;
      y1 = y2;
    }
  }
  cairo_stroke(cr);
}


void
_slope_funcplot_draw_triangles (slope_item_t *item, cairo_t *cr,
                                const slope_metrics_t *metrics)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;

  const double *vx = self->vx;
  const double *vy = self->vy;
  const int n = self->n;

  double x1 = slope_xymetrics_map_x(metrics, vx[0]);
  double y1 = slope_xymetrics_map_y(metrics, vy[0]);
  cairo_move_to(cr, x1 - TRI_RAD, y1 + TRI_RAD);
  cairo_line_to(cr, x1 + TRI_RAD, y1 + TRI_RAD);
  cairo_line_to(cr, x1, y1 - TRI_RAD);
  cairo_close_path(cr);
  if (self->fill_symbol) cairo_fill(cr);

  int k;
  for (k=1; k<n; k++) {
    double x2 = slope_xymetrics_map_x(metrics, vx[k]);
    double y2 = slope_xymetrics_map_y(metrics, vy[k]);

    double dx = x2 - x1;
    double dy = y2 - y1;
    double distsqr = dx*dx + dy*dy;

    if (distsqr >= TWO_TRI_RAD_SQR) {
      cairo_move_to(cr, x2 - TRI_RAD, y2 + TRI_RAD);
      cairo_line_to(cr, x2 + TRI_RAD, y2 + TRI_RAD);
      cairo_line_to(cr, x2, y2 + TRI_RAD);
      cairo_close_path(cr);
      if (self->fill_symbol) cairo_fill(cr);
      x1 = x2;
      y1 = y2;
    }
  }
  cairo_stroke(cr);
}


void
_slope_funcplot_draw_squares (slope_item_t *item, cairo_t *cr,
                              const slope_metrics_t *metrics)
{
  /* TODO */
}


void
_slope_funcplot_draw_plusses (slope_item_t *item, cairo_t *cr,
                              const slope_metrics_t *metrics)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;

  const double *vx = self->vx;
  const double *vy = self->vy;
  const int n = self->n;

  double x1 = slope_xymetrics_map_x(metrics, vx[0]);
  double y1 = slope_xymetrics_map_y(metrics, vy[0]);
  cairo_move_to(cr, x1 - PLUS_RAD, y1);
  cairo_line_to(cr, x1 + PLUS_RAD, y1);
  cairo_move_to(cr, x1, y1 - PLUS_RAD);
  cairo_line_to(cr, x1, y1 + PLUS_RAD);
  int k;
  for (k=1; k<n; k++) {
    double x2 = slope_xymetrics_map_x(metrics, vx[k]);
    double y2 = slope_xymetrics_map_y(metrics, vy[k]);

    double dx = x2 - x1;
    double dy = y2 - y1;
    double distsqr = dx*dx + dy*dy;

    if (distsqr >= TWO_PLUS_RAD_SQR) {
      cairo_move_to(cr, x1 - PLUS_RAD, y1);
      cairo_line_to(cr, x1 + PLUS_RAD, y1);
      cairo_move_to(cr, x1, y1 - PLUS_RAD);
      cairo_line_to(cr, x1, y1 + PLUS_RAD);
      x1 = x2;
      y1 = y2;
    }
  }
  cairo_stroke(cr);
}


void
_slope_funcplot_draw_line_circles (slope_item_t *item, cairo_t *cr,
                                   const slope_metrics_t *metrics)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;

  const double *vx = self->vx;
  const double *vy = self->vy;
  const int n = self->n;

  double x1 = slope_xymetrics_map_x(metrics, vx[0]);
  double y1 = slope_xymetrics_map_y(metrics, vy[0]);
  cairo_move_to(cr, x1 + CIRC_RAD, y1);
  cairo_arc(cr, x1, y1, CIRC_RAD, 0.0, 6.283185);
  if (self->fill_symbol) cairo_fill(cr);
  cairo_move_to(cr, x1, y1);

  int k;
  for (k=1; k<n; k++) {
    double x2 = slope_xymetrics_map_x(metrics, vx[k]);
    double y2 = slope_xymetrics_map_y(metrics, vy[k]);

    double dx = x2 - x1;
    double dy = y2 - y1;
    double distsqr = dx*dx + dy*dy;

    if (distsqr >= TWO_CIRC_RAD_SQR) {
      cairo_line_to(cr, x2, y2);
      cairo_stroke(cr);
      cairo_move_to(cr, x2 + CIRC_RAD, y2);
      cairo_arc(cr, x2, y2, CIRC_RAD, 0.0, 6.283185);
      if (self->fill_symbol) cairo_fill(cr);
      cairo_move_to(cr, x2, y2);
      x1 = x2;
      y1 = y2;
    }
  }
  cairo_stroke(cr);
}


void
_slope_funcplot_draw_line_triangles (slope_item_t *item, cairo_t *cr,
                                     const slope_metrics_t *metrics)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;

  const double *vx = self->vx;
  const double *vy = self->vy;
  const int n = self->n;

  double x1 = slope_xymetrics_map_x(metrics, vx[0]);
  double y1 = slope_xymetrics_map_y(metrics, vy[0]);
  cairo_move_to(cr, x1 - TRI_RAD, y1 + TRI_RAD);
  cairo_line_to(cr, x1 + TRI_RAD, y1 + TRI_RAD);
  cairo_line_to(cr, x1, y1 - TRI_RAD);
  cairo_close_path(cr);
  if (self->fill_symbol) cairo_fill(cr);
  cairo_move_to(cr, x1, y1);

  int k;
  for (k=1; k<n; k++) {
    double x2 = slope_xymetrics_map_x(metrics, vx[k]);
    double y2 = slope_xymetrics_map_y(metrics, vy[k]);

    double dx = x2 - x1;
    double dy = y2 - y1;
    double distsqr = dx*dx + dy*dy;

    if (distsqr >= TWO_TRI_RAD_SQR) {
      cairo_line_to(cr, x2, y2);
      cairo_stroke(cr);
      cairo_move_to(cr, x2 - TRI_RAD, y2 + TRI_RAD);
      cairo_line_to(cr, x2 + TRI_RAD, y2 + TRI_RAD);
      cairo_line_to(cr, x2, y2 - TRI_RAD);
      cairo_close_path(cr);
      if (self->fill_symbol) cairo_fill(cr);
      cairo_move_to(cr, x2, y2);
      x1 = x2;
      y1 = y2;
    }
  }
}


void
_slope_funcplot_draw_line_plusses (slope_item_t *item, cairo_t *cr,
                                   const slope_metrics_t *metrics)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;

  const double *vx = self->vx;
  const double *vy = self->vy;
  const int n = self->n;

  double x1 = slope_xymetrics_map_x(metrics, vx[0]);
  double y1 = slope_xymetrics_map_y(metrics, vy[0]);
  cairo_move_to(cr, x1 - PLUS_RAD, y1);
  cairo_line_to(cr, x1 - PLUS_RAD, y1);
  cairo_move_to(cr, x1, y1 - PLUS_RAD);
  cairo_line_to(cr, x1, y1 + PLUS_RAD);
  cairo_move_to(cr, x1, y1);

  int k;
  for (k=1; k<n; k++) {
    double x2 = slope_xymetrics_map_x(metrics, vx[k]);
    double y2 = slope_xymetrics_map_y(metrics, vy[k]);

    double dx = x2 - x1;
    double dy = y2 - y1;
    double distsqr = dx*dx + dy*dy;

    if (distsqr >= TWO_PLUS_RAD_SQR) {
      cairo_line_to(cr, x2, y2);
      cairo_stroke(cr);
      cairo_move_to(cr, x1 - PLUS_RAD, y1);
      cairo_line_to(cr, x1 + PLUS_RAD, y1);
      cairo_move_to(cr, x1, y1 - PLUS_RAD);
      cairo_line_to(cr, x1, y1 + PLUS_RAD);
      cairo_move_to(cr, x2, y2);
      x1 = x2;
      y1 = y2;
    }
  }
  cairo_stroke(cr);
}


void
_slope_funcplot_draw_thumb (slope_item_t *item,
                            const slope_point_t *pos, cairo_t *cr)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;

  slope_cairo_set_color(cr, &self->color);
  if (self->antialias) {
    cairo_set_antialias(
        cr, CAIRO_ANTIALIAS_SUBPIXEL);
  }
  else {
    cairo_set_antialias(
        cr, CAIRO_ANTIALIAS_NONE);
  }
  cairo_set_line_width(cr,self->line_width);

  switch (self->scatter)
  {
    case SLOPE_LINE:
      cairo_move_to(cr, pos->x - 10.0, pos->y - 3.0);
      cairo_line_to(cr, pos->x + 10.0, pos->y - 3.0);
      break;

    case SLOPE_CIRCLES:
      cairo_move_to(cr, pos->x + CIRC_RAD, pos->y - CIRC_RAD);
      cairo_arc(cr, pos->x, pos->y - CIRC_RAD, CIRC_RAD, 0.0, 6.283185);
      cairo_fill(cr);
      break;

    case SLOPE_PLUSSES:
      cairo_move_to(cr, pos->x - PLUS_RAD, pos->y - PLUS_RAD);
      cairo_line_to(cr, pos->x + PLUS_RAD, pos->y - PLUS_RAD);
      cairo_move_to(cr, pos->x , pos->y - PLUS_RAD - PLUS_RAD);
      cairo_line_to(cr, pos->x , pos->y + PLUS_RAD - PLUS_RAD);
      break;

    case SLOPE_LINE|SLOPE_CIRCLES:
      cairo_move_to(cr, pos->x + CIRC_RAD, pos->y - CIRC_RAD);
      cairo_arc(cr, pos->x, pos->y - CIRC_RAD, CIRC_RAD, 0.0, 6.283185);
      if (self->fill_symbol) cairo_fill(cr);
      cairo_move_to(cr, pos->x - 3.0*CIRC_RAD, pos->y - CIRC_RAD);
      cairo_line_to(cr, pos->x + 3.0*CIRC_RAD, pos->y - CIRC_RAD);
      break;

    case SLOPE_LINE|SLOPE_PLUSSES:
      cairo_move_to(cr, pos->x - 3.0*PLUS_RAD, pos->y - PLUS_RAD);
      cairo_line_to(cr, pos->x + 3.0*PLUS_RAD, pos->y - PLUS_RAD);
      cairo_move_to(cr, pos->x , pos->y - PLUS_RAD - PLUS_RAD);
      cairo_line_to(cr, pos->x , pos->y + PLUS_RAD - PLUS_RAD);
      break;
  }
  slope_draw_text(cr, item->font, pos->x + 17.0, pos->y, item->name);
  cairo_stroke(cr);
}


void
_slope_funcplot_check_ranges (slope_item_t *item)
{
  slope_funcplot_t *self = (slope_funcplot_t*) item;
  const double *vx = self->vx;
  const double *vy = self->vy;
  const int n = self->n;
  self->xmin = self->xmax = vx[0];
  self->ymin = self->ymax = vy[0];
  int k;
  for (k=1; k<n; k++) {
    if (vx[k] < self->xmin) self->xmin = vx[k];
    if (vx[k] > self->xmax) self->xmax = vx[k];
    if (vy[k] < self->ymin) self->ymin = vy[k];
    if (vy[k] > self->ymax) self->ymax = vy[k];
  }
}


void
slope_funcplot_set_antialias (slope_item_t *item, int on)
{
  if (item == NULL) {
    return;
  }
  slope_funcplot_t *self = (slope_funcplot_t*) item;
  self->antialias = on;
  slope_item_notify_appearence_change(item);
}


int
slope_funcplot_get_point_number (slope_item_t *item)
{
  if (item == NULL) return 0;
  slope_funcplot_t *self = (slope_funcplot_t*) item;
  return self->n;
}


const double*
slope_funcplot_get_x_array (slope_item_t *item)
{
  if (item == NULL) return NULL;
  slope_funcplot_t *self = (slope_funcplot_t*) item;
  return self->vx;
}


const double*
slope_funcplot_get_y_array (slope_item_t *item)
{
  if (item == NULL) return NULL;
  slope_funcplot_t *self = (slope_funcplot_t*) item;
  return self->vy;
}

/* slope/funcplot.c */
