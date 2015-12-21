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

#ifndef __SLOPE_ITEM_P__
#define __SLOPE_ITEM_P__

#include <slope/item.h>

#define SLOPE_ITEM_PRIVATE(instance) ((slope_item_private_t*) (instance))
#define SLOPE_ITEM_GET_PRIVATE(instance) SLOPE_ITEM_PRIVATE(SLOPE_ITEM(instance)->_private)

SLOPE_BEGIN_DECLS

typedef struct _slope_item_private slope_item_private_t;

struct _slope_item_private {
    slope_scale_t *scale;
    slope_bool_t visible;
    slope_bool_t has_thumb;
    char *name;
};

void _slope_item_draw (slope_item_t *self, cairo_t *cr);
void _slope_item_draw_thumb (slope_item_t *self, const slope_point_t *point, cairo_t *cr);
void _slope_item_draw_thumb_dummy_impl (slope_item_t *self, const slope_point_t *point, cairo_t *cr);
void _slope_item_set_scale (slope_item_t *self, slope_scale_t *scale);

SLOPE_END_DECLS

#endif /*__SLOPE_ITEM_P__*/
