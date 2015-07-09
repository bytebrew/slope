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


/*
***
*** WARNING Custom items are still in development, dont use them in any
*** `release` application, only if you want to play with slope's code
***
*/


#ifndef SLOPE_CUSTOM_ITEM_H
#define SLOPE_CUSTOM_ITEM_H

#include "slope/item.h"

SLOPE_BEGIN_DECLS

/**
 */
typedef void (*slope_draw_func_t) (slope_item_t *item,
                                   cairo_t *cr,
                                   const slope_metrics_t *metrics);


/**
 */
typedef void (*slope_draw_thumb_func_t) (slope_item_t *item,
                                         const slope_point_t *pos,
                                         cairo_t *cr);


/**
 */
slope_public slope_item_t* slope_custom_item_create();


/**
 */
slope_public void
slope_custom_item_register_draw_func (slope_item_t *self,
                                      slope_draw_func_t func);


/**
 */
slope_public void
slope_custom_item_register_draw_thumb_func (slope_item_t *self,
                                            slope_draw_thumb_func_t func);

SLOPE_END_DECLS

#endif /* SLOPE_CUSTOM_ITEM_H */
