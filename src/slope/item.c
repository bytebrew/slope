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

#include <slope/item_p.h>

typedef struct
_SlopeItemPrivate
{
    SlopeScene *scene;
    SlopeItem *parent;
    GList *child_list;
    gboolean owned;
}
SlopeItemPrivate;


#define SLOPE_ITEM_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_ITEM_TYPE, SlopeItemPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeItem,
    slope_item,
    G_TYPE_OBJECT)


static void _item_finalize (GObject *self);



static void
slope_item_class_init (SlopeItemClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->finalize = _item_finalize;

    klass->add_subitem = _item_add_subitem;
    klass->draw = _item_draw_rect;
    klass->mouse_event = _item_mouse_event;
    klass->get_scene_rect = _item_get_scene_rect;
}


static void
slope_item_init (SlopeItem *self)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);

    priv->scene = NULL;
    priv->parent = NULL;
    priv->child_list = NULL;
    priv->owned = FALSE;
}


static
void _item_finalize (GObject *self)
{

}


SlopeItem* slope_item_new (void)
{
    SlopeItem* self = SLOPE_ITEM(g_object_new(SLOPE_ITEM_TYPE, NULL));

    return self;
}


void _item_add_subitem (SlopeItem *self, SlopeItem *subitem, gboolean ownmem)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);

    if (subitem == NULL) {
        return;
    }

    _item_set_parent(subitem, self);
    _item_set_managed(subitem, ownmem);
    priv->child_list = g_list_append(priv->child_list, subitem);
}


void _item_set_parent (SlopeItem *self, SlopeItem *parent)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    priv->parent = parent;
}


void _item_get_scene_rect (SlopeItem *self, SlopeRect *rect)
{

}


void _item_mouse_event (SlopeItem *self, const SlopeMouseEvent *event)
{

}


void _item_set_scene (SlopeItem *self, SlopeScene *scene)
{

}


void _item_set_managed (SlopeItem *self, gboolean owned)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    priv->owned = owned;
}


void _item_draw_rect (SlopeItem *self, cairo_t *cr)
{
    slope_cairo_set_color(cr, SLOPE_RED);
    cairo_rectangle(cr, 20, 20, 50, 50);
    cairo_fill(cr);
}


void _item_draw_impl (SlopeItem *self, cairo_t *cr)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    GList *iter;

    SLOPE_ITEM_GET_CLASS(self)->draw(self, cr);
    iter = priv->child_list;
    while (iter != NULL) {
        _item_draw_impl(iter->data, cr);
        iter = iter->next;
    }
}

/* slope/item.c */
