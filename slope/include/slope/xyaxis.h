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

#ifndef SLOPE_XYAXIS_H
#define SLOPE_XYAXIS_H

#include <slope/item.h>
#include <slope/sampler.h>

#define SLOPE_XYAXIS_TYPE (slope_xyaxis_get_type())
#define SLOPE_XYAXIS(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), SLOPE_XYAXIS_TYPE, SlopeXyAxis))
#define SLOPE_XYAXIS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), SLOPE_XYAXIS_TYPE, SlopeXyAxisClass))
#define SLOPE_IS_XYAXIS(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), SLOPE_XYAXIS_TYPE))
#define SLOPE_IS_XYAXIS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), SLOPE_XYAXIS_TYPE))
#define SLOPE_XYAXIS_GET_CLASS(obj) \
  (SLOPE_XYAXIS_CLASS(G_OBJECT_GET_CLASS(obj)))

SLOPE_BEGIN_DECLS

typedef enum _SlopeXyAxisComponent {
  SLOPE_XYAXIS_LINE          = 0x00000001,
  SLOPE_XYAXIS_TICKS_UP      = 0x00000002,
  SLOPE_XYAXIS_TICKS_DOWN    = 0x00000004,
  SLOPE_XYAXIS_TITLE         = 0x00000008,
  SLOPE_XYAXIS_GRID          = 0x00000010,
  SLOPE_XYAXIS_ALL_COMPONENT = 0xFFFFFFFF,

  SLOPE_XYAXIS_DEFAULT_DOWN =
      SLOPE_XYAXIS_LINE | SLOPE_XYAXIS_TICKS_DOWN | SLOPE_XYAXIS_TITLE,

  SLOPE_XYAXIS_DEFAULT_UP =
      SLOPE_XYAXIS_LINE | SLOPE_XYAXIS_TICKS_UP | SLOPE_XYAXIS_TITLE,

  SLOPE_XYAXIS_DEFAULT_UP_GRID = SLOPE_XYAXIS_DEFAULT_UP | SLOPE_XYAXIS_GRID,

  SLOPE_XYAXIS_DEFAULT_DOWN_GRID = SLOPE_XYAXIS_DEFAULT_DOWN | SLOPE_XYAXIS_GRID
} SlopeXyAxisComponent;

typedef struct _SlopeXyAxis
{
  SlopeItem parent;
} SlopeXyAxis;

typedef struct _SlopeXyAxisClass
{
  SlopeItemClass parent_klass;
} SlopeXyAxisClass;

GType slope_xyaxis_get_type(void) G_GNUC_CONST;

SlopeItem *slope_xyaxis_new(int orientation, const char *title);

void slope_xyaxis_set_position(SlopeXyAxis *self,
                               double       min,
                               double       max,
                               double       anchor);

void slope_xyaxis_set_components(SlopeXyAxis *self, guint32 components);

void slope_xyaxis_set_title(SlopeXyAxis *self, const char *title);

const char *slope_xyaxis_get_title(SlopeXyAxis *self);

SlopeSampler *slope_xyaxis_get_sampler(SlopeXyAxis *self);

gboolean slope_xyaxis_get_selected(SlopeXyAxis *self);

void slope_xyaxis_set_selected(SlopeXyAxis *self, gboolean selected);

void slope_xyaxis_set_line_color(SlopeXyAxis *self, SlopeColor color);

SlopeColor slope_xyaxis_get_line_color(SlopeXyAxis *self);

void slope_xyaxis_set_grid_color(SlopeXyAxis *self, SlopeColor color);

SlopeColor slope_xyaxis_get_grid_color(SlopeXyAxis *self);

void slope_xyaxis_set_title_color(SlopeXyAxis *self, SlopeColor color);

SlopeColor slope_xyaxis_get_title_color(SlopeXyAxis *self);

void slope_xyaxis_set_selection_color(SlopeXyAxis *self, SlopeColor color);

SlopeColor slope_xyaxis_get_selection_color(SlopeXyAxis *self);

SLOPE_END_DECLS

#endif /* SLOPE_XYAXIS_H */
