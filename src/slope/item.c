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
#include <slope/figure.h>
#include <slope/view.h>

typedef struct
_SlopeItemPrivate
{
    SlopeFigure *figure;
    SlopeView *view;
    SlopeItem *parent;
    GList *child_list;
    gboolean owned;

    SlopeColor color;
    SlopeRect rect;
}
SlopeItemPrivate;


#define SLOPE_ITEM_GET_CLASS(obj) \
    (SLOPE_ITEM_CLASS(G_OBJECT_GET_CLASS(obj)))

#define SLOPE_ITEM_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_ITEM_TYPE, SlopeItemPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeItem,
    slope_item,
    G_TYPE_OBJECT)


static gboolean _item_mouse_event_color (SlopeItem *self, const SlopeMouseEvent *event);
static void _item_add_subitem (SlopeItem *self, SlopeItem *subitem, gboolean ownmem);
static void _item_set_parent (SlopeItem *self, SlopeItem *parent);
static void _item_get_figure_rect (SlopeItem *self, SlopeRect *rect);
static void _item_finalize (GObject *self);
static void _item_draw_rect (SlopeItem *self, cairo_t *cr);



static void
slope_item_class_init (SlopeItemClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->dispose = _item_finalize;

    klass->add_subitem = _item_add_subitem;
    klass->draw = _item_draw_rect;
    klass->mouse_event = _item_mouse_event_color;
    klass->get_figure_rect = _item_get_figure_rect;
}


static void
slope_item_init (SlopeItem *self)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);

    priv->figure = NULL;
    priv->parent = NULL;
    priv->child_list = NULL;
    priv->owned = FALSE;

    priv->color = SLOPE_BLUE;
    priv->rect.x = 10;
    priv->rect.y = 10;
    priv->rect.width = 100;
    priv->rect.height = 100;
}


static
void _item_finalize (GObject *self)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    GObjectClass *parent_class = g_type_class_peek_parent(G_OBJECT_GET_CLASS(self));
    GList *iter;

    /* destroy the managed children */
    iter = priv->child_list;
    while (iter != NULL) {
        if (_item_get_is_managed(SLOPE_ITEM(iter->data)) == TRUE) {
            g_object_unref(G_OBJECT(iter->data));
        }
        iter = iter->next;
    }
    priv->child_list = NULL;

    G_OBJECT_CLASS(parent_class)->finalize(self);
}


SlopeItem* slope_item_new (void)
{
    SlopeItem* self = SLOPE_ITEM(g_object_new(SLOPE_ITEM_TYPE, NULL));

    return self;
}


SlopeItem* slope_item_new_rect (double x, double y, double width, double height)
{
    SlopeItem* self = SLOPE_ITEM(g_object_new(SLOPE_ITEM_TYPE, NULL));
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);

    priv->rect.x = x;
    priv->rect.y = y;
    priv->rect.width = width;
    priv->rect.height = height;

    return self;
}


static
void _item_add_subitem (SlopeItem *self, SlopeItem *subitem, gboolean ownmem)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);

    if (subitem == NULL) {
        return;
    }

    _item_set_parent(subitem, self);
    _item_set_figure(subitem, priv->figure);
    _item_set_managed(subitem, ownmem);
    priv->child_list = g_list_append(priv->child_list, subitem);
}


void slope_item_add_subitem (SlopeItem *self, SlopeItem *subitem, gboolean ownmem)
{
    SLOPE_ITEM_GET_CLASS(self)->add_subitem(self, subitem, ownmem);
}


gboolean _item_get_is_managed (SlopeItem *self)
{
    return SLOPE_ITEM_GET_PRIVATE(self)->owned;
}


static
void _item_set_parent (SlopeItem *self, SlopeItem *parent)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    priv->parent = parent;
}


static
void _item_get_figure_rect (SlopeItem *self, SlopeRect *rect)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);

    if (rect != NULL) {
        rect->x = priv->rect.x;
        rect->y = priv->rect.y;
        rect->width = priv->rect.width;
        rect->height = priv->rect.height;
    }
}


static
gboolean _item_mouse_event_color (SlopeItem *self, const SlopeMouseEvent *event)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);

    if (event->type == SLOPE_MOUSE_LEFT_CLICK) {
        priv->color = SLOPE_RED;
    } else if (event->type == SLOPE_MOUSE_RIGHT_CLICK) {
        priv->color = SLOPE_BLUE;
    } else if (event->type == SLOPE_MOUSE_HOVER) {
        priv->color = g_random_int();
    }

    if (priv->view != NULL) {
        slope_view_redraw(priv->view);
    }

    return TRUE;
}


gboolean _item_mouse_event_impl (SlopeItem *self, const SlopeMouseEvent *event)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    GList *iter;
    gboolean caught;

    caught = FALSE;
    iter = priv->child_list;
    while (iter != NULL) {
        SlopeRect child_rect;

        /* if the event happens over any child, ask if it is responsible
         * for handling this event */
        slope_item_get_figure_rect(SLOPE_ITEM(iter->data), &child_rect);
        if (slope_rect_contains(&child_rect, event->x, event->y)) {
            if (_item_mouse_event_impl(SLOPE_ITEM(iter->data), event) == TRUE) {
                caught = TRUE;
            }
        }
        iter = iter->next;
    }

    /* only handle the event if no child has caught it */
    if (caught == FALSE) {
        return SLOPE_ITEM_GET_CLASS(self)->mouse_event(self, event);
    }

    return caught;
}


void slope_item_get_figure_rect (SlopeItem *self, SlopeRect *rect)
{
    SLOPE_ITEM_GET_CLASS(self)->get_figure_rect(self, rect);
}


void _item_set_figure (SlopeItem *self, SlopeFigure *figure)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    GList *iter;

    priv->figure = figure;
    priv->view = slope_figure_get_view(priv->figure);

    iter = priv->child_list;
    while (iter != NULL) {
        _item_set_figure(SLOPE_ITEM(iter->data), figure);
        iter = iter->next;
    }
}


void _item_set_managed (SlopeItem *self, gboolean owned)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    priv->owned = owned;
}


static
void _item_draw_rect (SlopeItem *self, cairo_t *cr)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    slope_cairo_set_color(cr, priv->color);
    slope_cairo_rect(cr, &priv->rect);
    cairo_fill(cr);
}


void _item_draw_impl (SlopeItem *self, cairo_t *cr)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    SlopeRect rect;
    GList *iter;

    /* only allow subitems inside this item's rect */
    cairo_save(cr);
    cairo_new_path(cr);
    slope_item_get_figure_rect(self, &rect);
    slope_cairo_rect(cr, &rect);
    cairo_clip(cr);

    /* first draw this item (it's in the back) and then
     * draw it's children (in front) */
    SLOPE_ITEM_GET_CLASS(self)->draw(self, cr);
    iter = priv->child_list;
    while (iter != NULL) {
        _item_draw_impl(iter->data, cr);
        iter = iter->next;
    }

    cairo_restore(cr);
}

/* slope/item.c */
