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

#ifndef SLOPE_SCALE_P_H
#define SLOPE_SCALE_P_H

#include <slope/scale.h>

void _scale_set_figure(SlopeScale *self, SlopeFigure *figure);

void _scale_draw(SlopeScale *self, const SlopeRect *rect, cairo_t *cr);

void _scale_handle_mouse_event(SlopeScale *self, SlopeMouseEvent *event);

void _scale_mouse_event_impl(SlopeScale *self, SlopeMouseEvent *event);

#endif /* SLOPE_SCALE_P_H */
