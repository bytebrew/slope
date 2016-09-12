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

#ifndef SLOPE_ITEM_P_H
#define SLOPE_ITEM_P_H

#include <slope/item.h>

void _item_add_subitem (SlopeItem *self, SlopeItem *subitem, gboolean ownmem);

void _item_get_scene_rect (SlopeItem *self, SlopeRect *rect);

void _item_mouse_event (SlopeItem *self, const SlopeMouseEvent *event);

void _item_set_scene (SlopeItem *self, SlopeScene *scene);

void _item_set_parent (SlopeItem *self, SlopeItem *parent);

void _item_set_managed (SlopeItem *self, gboolean owned);

void _item_draw_rect (SlopeItem *self, cairo_t *cr);

void _item_draw_impl (SlopeItem *self, cairo_t *cr);

#endif /* SLOPE_ITEM_P_H */
