/*
 * Copyright (C) 2017  Elvis Teixeira
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
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SLOPE_SCALE_H
#define SLOPE_SCALE_H

#include <slope/legend.h>

#define SLOPE_SCALE_TYPE (slope_scale_get_type())
#define SLOPE_SCALE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), SLOPE_SCALE_TYPE, SlopeScale))
#define SLOPE_SCALE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), SLOPE_SCALE_TYPE, SlopeScaleClass))
#define SLOPE_IS_SCALE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), SLOPE_SCALE_TYPE))
#define SLOPE_IS_SCALE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), SLOPE_SCALE_TYPE))
#define SLOPE_SCALE_GET_CLASS(obj) (SLOPE_SCALE_CLASS(G_OBJECT_GET_CLASS(obj)))

SLOPE_BEGIN_DECLS

typedef struct _SlopeScale
{
  GObject parent;

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
} SlopeScale;

typedef struct _SlopeScaleClass
{
  GObjectClass parent_class;

  void (*add_item)(SlopeScale *self, SlopeItem *item);
  void (*remove_item)(SlopeScale *self, SlopeItem *item);
  void (*draw)(SlopeScale *self, const SlopeRect *rect, cairo_t *cr);
  void (*map)(SlopeScale *self, SlopePoint *res, const SlopePoint *src);
  void (*unmap)(SlopeScale *self, SlopePoint *res, const SlopePoint *src);
  void (*rescale)(SlopeScale *self);
  void (*get_figure_rect)(SlopeScale *self, SlopeRect *rect);
  void (*get_data_rect)(SlopeScale *self, SlopeRect *rect);
  void (*mouse_event)(SlopeScale *self, SlopeMouseEvent *event);
  void (*position_legend)(SlopeScale *self);

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
} SlopeScaleClass;

GType slope_scale_get_type(void) G_GNUC_CONST;

void slope_scale_add_item(SlopeScale *self, SlopeItem *item);

void slope_scale_remove_item(SlopeScale *self, SlopeItem *item);

void slope_scale_remove_item_by_name(SlopeScale *self, const char *itemname);

GList *slope_scale_get_item_list(SlopeScale *self);

SlopeItem *slope_scale_get_item_by_name(SlopeScale *self, const char *itemname);

void slope_scale_map(SlopeScale *self, SlopePoint *res, const SlopePoint *src);

void slope_scale_unmap(SlopeScale *      self,
                       SlopePoint *      res,
                       const SlopePoint *src);

void slope_scale_rescale(SlopeScale *self);

void slope_scale_get_figure_rect(SlopeScale *self, SlopeRect *rect);

void slope_scale_get_data_rect(SlopeScale *self, SlopeRect *rect);

SlopeFigure *slope_scale_get_figure(SlopeScale *self);

SlopeItem *slope_scale_get_legend(SlopeScale *self);

SlopeView *slope_scale_get_view(SlopeScale *self);

gboolean slope_scale_get_is_managed(SlopeScale *self);

SlopeColor slope_scale_get_background_color(SlopeScale *self);

void slope_scale_set_background_color(SlopeScale *self, SlopeColor color);

void slope_scale_set_is_managed(SlopeScale *self, gboolean managed);

gboolean slope_scale_get_is_visible(SlopeScale *self);

void slope_scale_set_is_visible(SlopeScale *self, gboolean visible);

void slope_scale_get_layout_rect(SlopeScale *self, SlopeRect *rect);

void slope_scale_set_layout_rect(
    SlopeScale *self, double x, double y, double w, double h);

void slope_scale_set_name(SlopeScale *self, const char *name);

char *slope_scale_get_name(SlopeScale *self);

void slope_scale_set_show_name(SlopeScale *self, gboolean show);

void slope_scale_set_name_top_padding(SlopeScale *self, double padding);

void slope_scale_detach(SlopeScale *self);

gboolean slope_scale_get_show_name(SlopeScale *self);

SLOPE_END_DECLS

#endif /* SLOPE_SCALE_H */
