/*
 * Copyright (C) 2017  Elvis Teixeira
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
#include <slope/view.h>

void _item_draw(SlopeItem *self, cairo_t *cr);

void _item_draw_thumb(SlopeItem *self, cairo_t *cr, const SlopePoint *pos);

void _item_set_scale(SlopeItem *self, SlopeScale *scale);

void _item_handle_mouse_event(SlopeItem *self, SlopeMouseEvent *event);

void _item_mouse_event_impl(SlopeItem *self, SlopeMouseEvent *event);

#endif /* SLOPE_ITEM_P_H */
