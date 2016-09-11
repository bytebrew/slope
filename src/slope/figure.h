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

#ifndef SLOPE_FIGURE_H
#define SLOPE_FIGURE_H

#include <glib-object.h>
#include <slope/drawing.h>

SLOPE_BEGIN_DECLS

#define SLOPE_TYPE_FIGURE slope_figure_get_type()
G_DECLARE_DERIVABLE_TYPE (SlopeFigure, slope_figure, SLOPE, FIGURE, GObject)

struct _SlopeFigureClass
{
  GObjectClass parent_class;

  void (*draw) (SlopeFigure *self, const SlopeRect *rect, cairo_t *cr);
  void (*add_item) (SlopeFigure *self, SlopeItem *item);

  /* Padding to allow adding up to 8 new virtual functions
     without breaking ABI. */
  gpointer padding[8];
};


SlopeFigure* slope_figure_new (void);

SLOPE_END_DECLS

#endif /* SLOPE_FIGURE_H */
