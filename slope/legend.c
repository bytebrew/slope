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

#include "slope/legend_p.h"
#include "slope/figure_p.h"
#include "slope/metrics_p.h"
#include "slope/text.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


slope_item_class_t*
_slope_legend_get_class()
{
  static int first_call = SLOPE_TRUE;
  static slope_item_class_t klass;

  if (first_call) {
    klass.destroy_fn = NULL;
    klass.draw_fn = _slope_legend_draw;
    klass.draw_thumb_fn = NULL;
    first_call = SLOPE_FALSE;
  }

  return &klass;
}


slope_item_t*
slope_legend_create ()
{
  slope_legend_t *legend = malloc(sizeof(slope_legend_t));
  slope_item_t *parent = (slope_item_t*) legend;

  parent->klass = _slope_legend_get_class();
  parent->visible = SLOPE_TRUE;
  parent->name = NULL;
  parent->metrics = NULL;
  parent->has_thumb = SLOPE_FALSE;
  parent->font = NULL;
  legend->position = SLOPE_LEGEND_TOPRIGHT;
  legend->paint = SLOPE_TRUE;
  legend->stroke = SLOPE_FALSE;

  slope_color_set_name(&legend->fill_color, SLOPE_WHITE);
  slope_color_set_name(&legend->stroke_color, SLOPE_BLACK);

  return parent;
}


void
_slope_legend_eval_geometry (slope_item_t *item, cairo_t *cr,
                             const slope_metrics_t *metrics)
{
  slope_iterator_t *met_iter;
  slope_iterator_t *item_iter;
  slope_legend_t *self;
  slope_figure_t *figure;
  slope_rect_t data_rect, fig_rect;
  
  slope_metrics_get_data_rect(metrics, &data_rect);
  slope_metrics_get_figure_rect(metrics, &fig_rect);

  self = (slope_legend_t*) item;
  figure = slope_metrics_get_figure(metrics);

  double max_width = 0.0;
  self->rect.width = 0.0;
  self->rect.height = 0.0;

  /* for each metrics in the figure */
  SLOPE_LIST_FOREACH (met_iter, slope_figure_get_metrics_list(figure)) {
    slope_metrics_t *metrics = (slope_metrics_t*) slope_iterator_data(met_iter);
    /* for each item in the metrics */
    SLOPE_LIST_FOREACH (item_iter, slope_metrics_get_item_list(metrics)) {
      slope_item_t *item = (slope_item_t*) slope_iterator_data(item_iter);

      /* check if item is visible and has a legend thumb */
      if (slope_item_get_visible(item) == SLOPE_FALSE
          || slope_item_get_has_thumb(item) == SLOPE_FALSE) {
        continue;
      }

      /* increment height and check for bigger width */
      slope_rect_t txt_rec;
      slope_get_text_rect(cr, item->font, &txt_rec,
                          slope_item_get_name(item));
      self->rect.height += txt_rec.height + 4.0;
      if (txt_rec.width > max_width) max_width = txt_rec.width;
    }
  }

  self->rect.width = max_width + 40.0;
  self->rect.height += 10.0;

  switch (self->position) {
    case SLOPE_LEGEND_TOPRIGHT:
      self->rect.x = fig_rect.x + fig_rect.width - self->rect.width - 10.0;
      self->rect.y = fig_rect.y + 10.0;
      break;
    case SLOPE_LEGEND_TOPLEFT:
      self->rect.x = fig_rect.y + 10.0;
      self->rect.y = fig_rect.y + 10.0;
      break;
    case SLOPE_LEGEND_TOPRIGHT_OUTSIDE:
      self->rect.x = fig_rect.x + fig_rect.width + 200.0;
      self->rect.y = fig_rect.y + 10.0;
      break;
    default: /* TOPRIGHT */
        self->rect.x = fig_rect.x + fig_rect.width - self->rect.width - 10.0;
        self->rect.y = fig_rect.y + 10.0;
      break;
  }
}


void
_slope_legend_draw (slope_item_t *item, cairo_t *cr,
                    const slope_metrics_t *metrics)
{
  slope_iterator_t *met_iter;
  slope_iterator_t *item_iter;
  slope_legend_t *self;
  slope_figure_t *figure;
  slope_rect_t *rec;

  self = (slope_legend_t*) item;
  figure = slope_metrics_get_figure(metrics);
  rec = &self->rect;

  _slope_legend_eval_geometry(item, cr, metrics);

  /* fill background */
  if (self->paint) {
    slope_cairo_set_color(cr, &self->fill_color);
    cairo_rectangle(cr, rec->x, rec->y, rec->width, rec->height);
    cairo_fill(cr);
  }

  /* paint outline */
  if (self->stroke) {
    slope_cairo_set_color(cr, &self->stroke_color);
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
    cairo_set_line_width(cr, 1.0);
    cairo_rectangle(cr, rec->x, rec->y, rec->width, rec->height);
    cairo_stroke(cr);
  }

  /* finaly draw the legend entries */
  slope_point_t entry_pos;
  entry_pos.x = self->rect.x + 15.0;
  entry_pos.y = self->rect.y;

  /* for each metrics in the figure */
  SLOPE_LIST_FOREACH (met_iter, slope_figure_get_metrics_list(figure)) {
    slope_metrics_t *metrics = (slope_metrics_t*) slope_iterator_data(met_iter);
    /* for each item in the metrics */
    SLOPE_LIST_FOREACH (item_iter, slope_metrics_get_item_list(metrics)) {
      slope_item_t *item = (slope_item_t*) slope_iterator_data(item_iter);

      const char *entry = slope_item_get_name(item);

      /* check if item is visible and has a legend thumb */
      if (slope_item_get_visible(item) == SLOPE_FALSE
          || slope_item_get_has_thumb(item) == SLOPE_FALSE) {
        continue;
      }

      /* position pen and delegate entry drawing to the item */
      slope_rect_t txt_rec;
      slope_get_text_rect(cr, item->font, &txt_rec, entry);
      entry_pos.y += txt_rec.height + 4.0;
      _slope_item_draw_thumb(item, &entry_pos, cr);
    }
  }
  cairo_stroke(cr);
}

/* slope/legend.c */
