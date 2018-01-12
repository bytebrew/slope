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

#ifndef SLOPE_LEGEND_H
#define SLOPE_LEGEND_H

#include <slope/item.h>

#define SLOPE_LEGEND_TYPE (slope_legend_get_type())
#define SLOPE_LEGEND(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), SLOPE_LEGEND_TYPE, SlopeLegend))
#define SLOPE_LEGEND_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), SLOPE_LEGEND_TYPE, SlopeLegendClass))
#define SLOPE_IS_LEGEND(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), SLOPE_LEGEND_TYPE))
#define SLOPE_IS_LEGEND_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), SLOPE_LEGEND_TYPE))
#define SLOPE_LEGEND_GET_CLASS(obj) \
  (SLOPE_LEGEND_CLASS(G_OBJECT_GET_CLASS(obj)))

SLOPE_BEGIN_DECLS

typedef enum _SlopeLegendPosition {
  SLOPE_LEGEND_CUSTOM,
  SLOPE_LEGEND_TOP,
  SLOPE_LEGEND_BOTTOM,
  SLOPE_LEGEND_LEFT,
  SLOPE_LEGEND_RIGHT,
  SLOPE_LEGEND_TOPLEFT,
  SLOPE_LEGEND_TOPRIGHT,
  SLOPE_LEGEND_BOTTOMLEFT,
  SLOPE_LEGEND_BOTTOMRIGHT
} SlopeLegendPosition;

typedef struct _SlopeLegend
{
  SlopeItem parent;

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
} SlopeLegend;

typedef struct _SlopeLegendClass
{
  SlopeItemClass parent_class;

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
} SlopeLegendClass;

GType slope_legend_get_type(void) G_GNUC_CONST;

SlopeItem *slope_legend_new(SlopeOrientation orientation);

SlopeColor slope_legend_get_fill_color(SlopeLegend *self);

void slope_legend_set_fill_color(SlopeLegend *self, SlopeColor color);

SlopeColor slope_legend_get_stroke_color(SlopeLegend *self);

void slope_legend_set_stroke_color(SlopeLegend *self, SlopeColor color);

void slope_legend_set_stroke_width(SlopeLegend *self, double width);

double slope_legend_get_stroke_width(SlopeLegend *self);

void slope_legend_set_orientation(SlopeLegend *    self,
                                  SlopeOrientation orientation);

SlopeOrientation slope_legend_get_orientation(SlopeLegend *self);

void slope_legend_set_anchor(SlopeLegend *self, SlopeCorner anchor);

SlopeCorner slope_legend_get_anchor(SlopeLegend *self);

void slope_legend_set_position(SlopeLegend *self, double x, double y);

void slope_legend_get_position(SlopeLegend *self, double *x, double *y);

void slope_legend_set_default_position(SlopeLegend *       self,
                                       SlopeLegendPosition position);

void slope_legend_add_item(SlopeLegend *self, SlopeItem *item);

void slope_legend_clear_items(SlopeLegend *self);

SLOPE_END_DECLS

#endif /* SLOPE_LEGEND_H */
