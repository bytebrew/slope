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

#include "slope/custom-item_p.h"
#include <stdlib.h>


/**
 *
 */
static slope_item_class_t*
_slope_custom_item_get_class()
{
    static int first_call = SLOPE_TRUE;
    static slope_item_class_t klass;

    if (first_call) {
        klass.destroy_fn = NULL;
        klass.draw_fn = _slope_custom_draw;
        klass.draw_thumb_fn = _slope_custom_draw_thumb;
        klass.get_rect_fn = _slope_custom_get_rect;
        first_call = SLOPE_FALSE;
    }

    return &klass;
}

/**
 *
 */
slope_item_t*
slope_custom_item_create()
{
    slope_custom_item_t *custom_item = malloc(sizeof(slope_custom_item_t));
    slope_item_t *item = (slope_item_t*) custom_item;

    item->klass = _slope_custom_item_get_class();
    item->metrics = NULL;
    item->name = NULL;
    item->font = NULL;
    item->visible = SLOPE_TRUE;
    item->has_thumb = SLOPE_FALSE;

    custom_item->draw_fn = NULL;
    custom_item->draw_thumb_fn = NULL;
    custom_item->get_rect_fn = NULL;

    return item;
}

/**
 *
 */
void
slope_custom_item_register_draw_func (slope_item_t *item,
                                      slope_draw_func_t func)
{
  slope_custom_item_t *self = (slope_custom_item_t*) item;
  if (self == NULL) return;
  self->draw_fn = func;
}

/**
 *
 */
void
slope_custom_item_register_draw_thumb_func (slope_item_t *item,
                                            slope_draw_thumb_func_t func)
{
  slope_custom_item_t *self = (slope_custom_item_t*) item;
  if (self == NULL) return;
  self->draw_thumb_fn = func;
}

/**
 *
 */
void
slope_custom_item_register_get_rect_func (slope_item_t *item,
                                          slope_get_rect_func_t func)
{
  slope_custom_item_t *self = (slope_custom_item_t*) item;
  if (self == NULL) return;
  self->get_rect_fn = func;
}

/**
 *
 */
void _slope_custom_draw (slope_item_t *item, cairo_t *cr,
                         const slope_metrics_t *metrics)
{
  slope_custom_item_t *self = (slope_custom_item_t*) item;
  if (self->draw_fn != NULL) self->draw_fn(item, cr, metrics);
}

/**
 *
 */
void _slope_custom_draw_thumb (slope_item_t *item,
                               const slope_point_t *pos,
                               cairo_t *cr)
{
  slope_custom_item_t *self = (slope_custom_item_t*) item;
  if (self->draw_fn != NULL) self->draw_thumb_fn(item, pos, cr);
}

/**
 *
 */
void _slope_custom_get_rect (slope_item_t *item,
                             slope_rect_t *rect)
{
  slope_custom_item_t *self = (slope_custom_item_t*) item;
  if (self->draw_fn != NULL) self->get_rect_fn(item, rect);
}

/* slope/custom-item.c */
