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

#ifndef SLOPE_CUSTOM_ITEM_P_H
#define SLOPE_CUSTOM_ITEM_P_H

#include "custom-item.h"
#include "slope/item_p.h"

SLOPE_BEGIN_DECLS

/**
 *
 */
typedef struct _slope_custom_item slope_custom_item_t;

/**
 *
 */
struct _slope_custom_item
{
  slope_item_t            parent;
  slope_draw_func_t       draw_fn;
  slope_draw_thumb_func_t draw_thumb_fn;
  slope_get_rect_func_t   get_rect_fn;
};



void _slope_custom_draw (slope_item_t *item, cairo_t *cr,
                         const slope_metrics_t *metrics);


void _slope_custom_draw_thumb (slope_item_t *item,
                               const slope_point_t *pos,
                               cairo_t *cr);


void _slope_custom_get_rect (slope_item_t *item,
                             slope_rect_t *rect);

SLOPE_END_DECLS

#endif /* SLOPE_CUSTOM_ITEM_P_H */
