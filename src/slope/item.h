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
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SLOPE_ITEM__
#define __SLOPE_ITEM__

#include <slope/drawing.h>

#define SLOPE_ITEM(instance) ((slope_item_t*) (instance))
#define SLOPE_ITEM_CLASS(instance) ((slope_item_class_t*) (instance))
#define SLOPE_ITEM_GET_CLASS(instance) SLOPE_ITEM_CLASS(SLOPE_ITEM(instance)->_class)

SLOPE_BEGIN_DECLS

struct _slope_item_class {
   void (*init) (slope_item_t*);
   void (*finalize) (slope_item_t*);
   void (*draw) (slope_item_t*, cairo_t*);
   void (*draw_thumb) (slope_item_t*, const slope_point_t*, cairo_t*);
   void (*get_data_rect) (const slope_item_t*, slope_rect_t*);
   void (*get_figure_rect) (const slope_item_t*, slope_rect_t*);
};


struct _slope_item {
   struct _slope_item_class *_class;
   void *_private;
};


void slope_item_destroy (slope_item_t *self);
void slope_item_init (slope_item_t *self);
void slope_item_finalize (slope_item_t *self);
slope_bool_t slope_item_get_visible (const slope_item_t *self);
slope_bool_t slope_item_has_thumb (const slope_item_t *self);
void slope_item_set_name (slope_item_t *self, const char *name);
const char* slope_item_get_name (const slope_item_t *self);
void slope_item_get_data_rect (const slope_item_t *self, slope_rect_t *rect);
void slope_item_get_figure_rect (const slope_item_t *self, slope_rect_t *rect);

SLOPE_END_DECLS

#endif /*__SLOPE_ITEM__*/
