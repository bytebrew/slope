/*
 * Copyright (C) 2018  Elvis Teixeira
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

#include "slope/pen_p.h"

void slope_pen_destroy(slope_pen_t *self)
{
  self->klass->destroy(self);
}

void slope_pen_begin_path(slope_pen_t *self)
{
  self->klass->begin_path(self);
}

void slope_pen_end_path(slope_pen_t *self)
{
  self->klass->end_path(self);
}

void slope_pen_move_to(slope_pen_t *self, slope_float_t x, slope_float_t y)
{
  self->klass->move_to(self, x, y);
}

void slope_pen_line_to(slope_pen_t *self, slope_float_t x, slope_float_t y)
{
  self->klass->line_to(self, x, y);
}

void slope_pen_close_path(slope_pen_t *self)
{
  self->klass->close_path(self);
}

void slope_pen_set_colors(slope_pen_t * self,
                          slope_color_t stroke,
                          slope_color_t fill)
{
  self->klass->set_colors(self, stroke, fill);
}

void slope_pen_circle(slope_pen_t * self,
                      slope_float_t x,
                      slope_float_t y,
                      slope_float_t r)
{
  self->klass->circle(self, x, y, r);
}

void slope_pen_set_width(slope_pen_t *self, slope_float_t width)
{
  self->klass->set_width(self, width);
}

void slope_pen_text(slope_pen_t * self,
                    slope_float_t x,
                    slope_float_t y,
                    const char *  text)
{
  self->klass->text(self, x, y, text);
}

/* slope/pen.c */
