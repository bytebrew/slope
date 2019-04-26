/*
 * Copyright (C) 2019  Elvis Teixeira
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

#ifndef SLOPE_ITEM_H
#define SLOPE_ITEM_H

#if !defined (SLOPE_H_INSIDE) && !defined (SLOPE_COMPILATION)
#error "Only <slope.h> can be included directly."
#endif

#include <cairo/cairo.h>
#include <glib-object.h>
#include <slope/drawing.h>

G_BEGIN_DECLS

#define SLOPE_TYPE_ITEM slope_item_get_type ()
G_DECLARE_DERIVABLE_TYPE (SlopeItem, slope_item, SLOPE, ITEM, GObject)


typedef struct {
    SlopeRect parent_rect;
    cairo_t *cr;
} SlopeDC;


struct _SlopeItemClass
{
  GObjectClass parent_class;
  void (*draw) (SlopeItem *self, SlopeDC *dc);

  /* Padding to allow adding up to 16 new virtual
   * functions without breaking ABI. */
  gpointer padding[16];
};


gboolean slope_item_is_visible(SlopeItem *self);

SlopeItem* slope_item_append (SlopeItem *self, SlopeItem *child);

G_END_DECLS

#endif /* SLOPE_ITEME_H */
