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

#ifndef SLOPE_SCENE_H
#define SLOPE_SCENE_H

#include <glib-object.h>
#include <slope/drawing.h>

#define SLOPE_SCENE_TYPE              (slope_scene_get_type())
#define SLOPE_SCENE(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SLOPE_SCENE_TYPE, SlopeScene))
#define SLOPE_SCENE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SLOPE_SCENE_TYPE, SlopeSceneClass))
#define SLOPE_SCENE_GET_CLASS(obj)    (SLOPE_SCENE_CLASS(G_OBJECT_GET_CLASS(obj)))
#define SLOPE_IS_SCENE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SLOPE_SCENE_TYPE))
#define SLOPE_IS_SCENE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), SLOPE_SCENE_TYPE))

SLOPE_BEGIN_DECLS

struct _SlopeScene
{
  GObject parent;

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
};


typedef struct
_SlopeSceneClass
{
  GObjectClass parent_class;

  void (*draw) (SlopeScene *self, const SlopeRect *rect, cairo_t *cr);
  void (*add_item) (SlopeScene *self, SlopeItem *item);

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
}
SlopeSceneClass;


GType slope_scene_get_type(void) G_GNUC_CONST;

SlopeScene* slope_scene_new (void);

GList* slope_scene_get_item_list (SlopeScene *self);

void slope_scene_add_item (SlopeScene *self, SlopeItem *item);

SlopeColor slope_scene_get_background_color (SlopeScene *self);

void slope_scene_set_background_color (SlopeScene *self, SlopeColor color);

void slope_scene_draw (SlopeScene *self, const SlopeRect *rect, cairo_t *cr);

void slope_scene_write_to_png (SlopeScene *self, const char *filename, int width, int height);

SlopeView* slope_scene_get_view (SlopeScene *self);

SLOPE_END_DECLS

#endif /* SLOPE_SCENE_H */
