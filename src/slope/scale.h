/*
 * Copyright (C) 2016  Elvis Teixeira
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

#include <slope/drawing.h>
#include <glib-object.h>

#define SLOPE_SCALE_TYPE              (slope_scale_get_type())
#define SLOPE_SCALE(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SLOPE_SCALE_TYPE, SlopeScale))
#define SLOPE_SCALE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SLOPE_SCALE_TYPE, SlopeScaleClass))
#define SLOPE_IS_SCALE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SLOPE_SCALE_TYPE))
#define SLOPE_IS_SCALE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), SLOPE_SCALE_TYPE))

SLOPE_BEGIN_DECLS

typedef struct
_SlopeScale
{
  GObject parent;

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
}
SlopeScale;


typedef struct
_SlopeScaleClass
{
  GObjectClass parent_class;

  void (*add_item) (SlopeScale *self, SlopeItem *item);
  void (*draw) (SlopeScale *self, const SlopeRect *rect, cairo_t *cr);
  void (*map) (SlopeScale *self, SlopePoint *res, const SlopePoint *src);
  void (*unmap) (SlopeScale *self, SlopePoint *res, const SlopePoint *src);
  void (*rescale) (SlopeScale *self);


  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
}
SlopeScaleClass;



GType slope_scale_get_type (void) G_GNUC_CONST;

SlopeScale* slope_scale_new (void);

void slope_scale_add_item (SlopeScale *self, SlopeItem *item);

void slope_scale_map (SlopeScale *self, SlopePoint *res, const SlopePoint *src);

void slope_scale_unmap (SlopeScale *self, SlopePoint *res, const SlopePoint *src);

void slope_scale_rescale (SlopeScale *self);

gboolean slope_scale_get_is_managed (SlopeScale *self);

void slope_scale_set_is_managed (SlopeScale *self, gboolean managed);

gboolean slope_scale_get_is_visible (SlopeScale *self);

void slope_scale_set_is_visible (SlopeScale *self, gboolean visible);

SLOPE_END_DECLS

#endif /* SLOPE_SCALE_H */
