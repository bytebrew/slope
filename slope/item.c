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

#include "slope/item_p.h"

G_DEFINE_TYPE_WITH_PRIVATE (SlopeItem, slope_item, G_TYPE_OBJECT)

static void item_draw (SlopeItem *self, SlopeDC *dc);
static void item_dispose (GObject *gobject);

static void
slope_item_class_init (SlopeItemClass *klass) {
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);
    klass->draw = item_draw;
    object_klass->dispose = item_dispose;
}


static void
slope_item_init (SlopeItem *self) {
  SlopeItemPrivate *priv = slope_item_get_instance_private (self);

  SLOPE_TREE_INIT (SLOPE_TREE (priv));
  priv->flags = ITEM_VISIBLE;
  priv->item = self;
}


static void
item_dispose (GObject *gobject) {
  G_OBJECT_CLASS (slope_item_parent_class)->dispose (gobject);
}


static void
item_draw (SlopeItem *self, SlopeDC *dc) {
    (void) self;
    (void) dc;
}


gboolean
slope_item_is_visible (SlopeItem *self) {
    SlopeItemPrivate *priv = slope_item_get_instance_private (self);
    return slope_enabled(priv->flags, ITEM_VISIBLE);
}


SlopeItem*
slope_item_append (SlopeItem *self, SlopeItem *child) {
    SlopeItemPrivate *priv = slope_item_get_instance_private (self);
    SlopeItemPrivate *child_priv = slope_item_get_instance_private (child);

    slope_tree_append (SLOPE_TREE (priv), SLOPE_TREE (child_priv));
    return child;
}
