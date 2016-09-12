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

#ifndef SLOPE_ITEM_H
#define SLOPE_ITEM_H

#include <glib-object.h>
#include <slope/drawing.h>
#include <slope/mouse.h>

#define SLOPE_ITEM_TYPE              (slope_item_get_type())
#define SLOPE_ITEM(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SLOPE_ITEM_TYPE, SlopeItem))
#define SLOPE_ITEM_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SLOPE_ITEM_TYPE, SlopeItemClass))
#define SLOPE_ITEM_GET_CLASS(obj)    (SLOPE_ITEM_CLASS(G_OBJECT_GET_CLASS(obj)))
#define SLOPE_IS_ITEM(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SLOPE_ITEM_TYPE))
#define SLOPE_IS_ITEM_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), SLOPE_ITEM_TYPE))

SLOPE_BEGIN_DECLS

struct _SlopeItem
{
  GObject parent;

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
};


typedef struct
_SlopeItemClass
{
  GObjectClass parent_class;

  void (*draw) (SlopeItem *self, cairo_t *cr);
  void (*add_subitem) (SlopeItem *self, SlopeItem *subitem, gboolean ownmem);
  void (*get_scene_rect) (SlopeItem *self, SlopeRect *rect);
  gboolean (*mouse_event) (SlopeItem *self, const SlopeMouseEvent *event);

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
}
SlopeItemClass;


GType slope_item_get_type (void) G_GNUC_CONST;

SlopeItem* slope_item_new (void);

SlopeItem* slope_item_new_rect (double x, double y, double width, double height);

GList slope_item_get_subitem_list (SlopeItem *self);

void slope_item_add_subitem (SlopeItem *self, SlopeItem *subitem, gboolean ownmem);

gboolean slope_item_get_is_visible (SlopeItem *self);

void slope_item_set_is_visible (SlopeItem *self, gboolean visible);

SlopeItem* slope_item_get_parent (SlopeItem *self);

SlopeScene* slope_item_get_scene (SlopeItem *self);

void slope_item_get_scene_rect (SlopeItem *self, SlopeRect *rect);

SLOPE_END_DECLS

#endif /* SLOPE_ITEM_H */
