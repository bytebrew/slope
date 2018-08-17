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

#include "slope/item_p.h"


G_DEFINE_TYPE_WITH_PRIVATE (SlopeItem, slope_item, G_TYPE_OBJECT)



/* local decls */
static void slope_item_finalize (GObject *self);
static void slope_item_dispose (GObject *self);


static void
slope_item_class_init (SlopeItemClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose = slope_item_dispose;
    gobject_class->finalize = slope_item_finalize;
}


static void
slope_item_init (SlopeItem *self)
{
    SlopeItemPrivate *m = SLOPE_ITEM_GET_PRIVATE (self);

    m->figure = NULL;
    slope_tree_init(&m->tree_node);
    m->publ_obj = self;
    m->text = slope_text_new("Droid Sans Mono 11");
}


static void
slope_item_dispose (GObject *object)
{
  //SlopeItem *self = SLOPE_ITEM (object);
  //SlopeItemPrivate *m = SLOPE_ITEM_GET_PRIVATE (self);

  G_OBJECT_CLASS (slope_item_parent_class)->dispose (object);
}


static void
slope_item_finalize (GObject *object)
{
    //SlopeItem *self = SLOPE_ITEM (object);
    //SlopeItemPrivate *m = SLOPE_ITEM_GET_PRIVATE (self);

    G_OBJECT_CLASS (slope_item_parent_class)->finalize (object);
}


SlopeItem*
slope_item_new (void)
{
    return SLOPE_ITEM (g_object_new (SLOPE_TYPE_ITEM, NULL));
}


void draw_item_p (SlopeItemPrivate *m, cairo_t *cr, const SlopeRect *rec)
{
    SlopeItem *self = m->publ_obj;
    SLOPE_ITEM_GET_CLASS (self)->draw(self, cr, rec);
}

/* slope/item.c */
