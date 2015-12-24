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

#ifndef __SLOPE_SCALE__
#define __SLOPE_SCALE__

#include <slope/drawing.h>
#include <slope/list.h>

#define SLOPE_SCALE(instance) ((slope_scale_t*) (instance))
#define SLOPE_SCALE_CLASS(instance) ((slope_scale_class_t*) (instance))
#define SLOPE_SCALE_GET_CLASS(instance) SLOPE_SCALE_CLASS(SLOPE_SCALE(instance)->_class)

SLOPE_BEGIN_DECLS

struct _slope_scale_class {
   void (*init) (slope_scale_t*);
   void (*finalize) (slope_scale_t*);
   void (*draw) (slope_scale_t*, const slope_rect_t*, cairo_t*);
   void (*map) (slope_scale_t*, slope_point_t*, const slope_point_t*);
   void (*get_data_rect) (const slope_scale_t*, slope_rect_t*);
   void (*get_figure_rect) (const slope_scale_t*, slope_rect_t*);
   void (*update) (slope_scale_t*);
   void (*update_for_item) (slope_scale_t*, slope_item_t*);
   void (*set_color_scheme) (slope_scale_t*, slope_color_t, slope_color_t, slope_color_t);
};


struct _slope_scale {
   struct _slope_scale_class *_class;
   void *_private;
};


void slope_scale_destroy (slope_scale_t *self);
void slope_scale_init (slope_scale_t *self);
void slope_scale_finalize (slope_scale_t *self);
void slope_scale_add_item (slope_scale_t *self, slope_item_t *item);
slope_list_t* slope_scale_get_item_list (const slope_scale_t *self);
slope_figure_t* slope_scale_get_figure (const slope_scale_t *self);
slope_bool_t slope_scale_get_visible (const slope_scale_t *self);
void slope_scale_map (slope_scale_t *self, slope_point_t *figure_point, const slope_point_t *data_point);
void slope_scale_get_data_rect (const slope_scale_t *self, slope_rect_t *rect);
void slope_scale_get_figure_rect (const slope_scale_t *self, slope_rect_t *rect);
void slope_scale_update (slope_scale_t *self);
void slope_scale_update_for_item (slope_scale_t *self, slope_item_t *item);
void slope_scale_set_color_scheme (slope_scale_t *self, slope_color_t background,
                                   slope_color_t foreground, slope_color_t extra_color);

SLOPE_END_DECLS

#endif /*__SLOPE_SCALE__*/
