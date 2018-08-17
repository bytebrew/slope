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

#ifndef _slope_item_h_
#define _slope_item_h_

#include <glib-object.h>
#include <cairo.h>
#include "slope/drawing.h"
#include "slope/text.h"

G_BEGIN_DECLS

#define SLOPE_TYPE_ITEM                 (slope_item_get_type ())
#define SLOPE_ITEM(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLOPE_TYPE_ITEM, SlopeItem))
#define SLOPE_ITEM_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SLOPE_TYPE_ITEM, SlopeItemClass))
#define SLOPE_IS_ITEM(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SLOPE_TYPE_ITEM))
#define SLOPE_IS_ITEM_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), SLOPE_TYPE_ITEM))
#define SLOPE_ITEM_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), SLOPE_TYPE_ITEM, SlopeItemClass))

typedef struct _SlopeItemClass        SlopeItemClass;
typedef struct _SlopeItemDC           SlopeItemDC;


/* The item drawing context */
struct _SlopeItemDC
{
    SlopeFigure       *figure;
    SlopeItem         *parent;
    cairo_t           *cr;
    const SlopeRect   *parent_rect;
    SlopeText         *default_text;
};


struct _SlopeItem
{
  /*< parent object space >*/
  GObject parent;

  /*< private >*/
  gpointer dummy;
};


struct _SlopeItemClass
{
  GObjectClass parent_class;

  void (*added) (SlopeItem *self, SlopeItem *parent, SlopeFigure *figure);
  void (*draw) (SlopeItem *self, const SlopeItemDC *dc);

  /* Padding for future expansion */
  void (*_slope_reserved1) (void);
  void (*_slope_reserved2) (void);
  void (*_slope_reserved3) (void);
  void (*_slope_reserved4) (void);
};


GType slope_item_get_type (void) G_GNUC_CONST;

SlopeItem* slope_item_new (void);

void slope_item_append (SlopeItem *parent, SlopeItem *child);

void slope_item_destroy_tree (SlopeItem *self);

G_END_DECLS

#endif /* _slope_item_h_ */
