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

#include "slope/figure_p.h"
#include "slope/item_p.h"

G_DEFINE_TYPE_WITH_PRIVATE (SlopeItem, slope_item, G_TYPE_OBJECT)


/* signals */
enum {
    SIG_ADD,
    SIG_REMOVE,
    SIG_CHANGED,
    SIG_LAST
};
static guint item_signals[SIG_LAST] = { 0 };


/* properties */
enum {
  PROP_0,
  PROP_VISIBLE,
  PROP_LAST
};
static GParamSpec *item_props[PROP_LAST] = { NULL };


/* local decls */
static void slope_item_finalize (GObject *self);
static void slope_item_dispose (GObject *self);
static void slope_item_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void slope_item_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);


static void
slope_item_class_init (SlopeItemClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose = slope_item_dispose;
    gobject_class->finalize = slope_item_finalize;
    gobject_class->set_property = slope_item_set_property;
    gobject_class->get_property = slope_item_get_property;

    klass->added = NULL;
    klass->draw = NULL;

    item_props[PROP_VISIBLE] =
          g_param_spec_boolean ("visible",
                                "Background fill color",
                                "Specify the background fill color",
                                TRUE,
                                G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);

    g_object_class_install_properties (gobject_class, PROP_LAST, item_props);
}


static void
slope_item_set_property (GObject *object, guint prop_id,
                         const GValue *value, GParamSpec *pspec)
{
    SlopeItemPrivate *m = SLOPE_ITEM_GET_PRIVATE(object);
    SLOPE_UNUSED(pspec);

    switch (prop_id) {
        case PROP_VISIBLE:
            slope_enable_if(m->options, g_value_get_boolean(value), ItemVisible);
            break;
    }
}


static void
slope_item_get_property (GObject *object, guint prop_id,
                           GValue *value, GParamSpec *pspec)
{
    SlopeItemPrivate *m = SLOPE_ITEM_GET_PRIVATE(object);
    SLOPE_UNUSED(pspec);

    switch (prop_id) {
        case PROP_VISIBLE:
            g_value_set_boolean(value, slope_enabled(m->options, ItemVisible));
            break;
    }
}


static void
slope_item_init (SlopeItem *self)
{
    SlopeItemPrivate *m = SLOPE_ITEM_GET_PRIVATE (self);

    m->figure = NULL;
    slope_tree_init(&m->tree_node);
    m->publ_obj = self;
    m->options = ItemVisible;
}


static void
slope_item_dispose (GObject *object)
{
    G_OBJECT_CLASS (slope_item_parent_class)->dispose (object);
}


static void
slope_item_finalize (GObject *object)
{
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
    SlopeItemPrivate *parent_p = SLOPE_ITEM_PRIVATE (SLOPE_TREE (m)->parent);
    SlopeFigurePrivate *fig_p = SLOPE_FIGURE_GET_PRIVATE(m->figure);
    SlopeItemDC dc;

    g_return_if_fail (SLOPE_IS_ITEM (self));
    g_return_if_fail (SLOPE_ITEM_GET_CLASS (self)->draw != NULL);

    dc.parent = parent_p ? parent_p->publ_obj : NULL;
    dc.figure = m->figure;
    dc.cr = cr;
    dc.default_text = fig_p->text;
    dc.parent_rect = rec;

    SLOPE_ITEM_GET_CLASS (self)->draw(self, &dc);
}


void slope_item_append (SlopeItem *parent, SlopeItem *child)
{
    SlopeItemPrivate *parent_p;
    SlopeItemPrivate *child_p;
    SlopeItemClass *child_class;

    g_return_if_fail (SLOPE_IS_ITEM (parent));
    g_return_if_fail (SLOPE_IS_ITEM (child));

    parent_p = SLOPE_ITEM_GET_PRIVATE (parent);
    child_p = SLOPE_ITEM_GET_PRIVATE (child);
    child_class = SLOPE_ITEM_GET_CLASS (child);

    child_p->figure = parent_p->figure;
    slope_tree_append (SLOPE_TREE (parent_p), SLOPE_TREE (child_p));

    if (child_class->added) {
        child_class->added (child, parent, parent_p->figure);
    }
}


static gpointer
tree_cleanup (gpointer data, gpointer context)
{
    SlopeItem *item = SLOPE_ITEM_PRIVATE (data)->publ_obj;
    SLOPE_UNUSED(context);

    g_object_unref (G_OBJECT (item));
    return NULL;
}


void
slope_item_destroy_tree (SlopeItem *self)
{
    SlopeItemPrivate *m;

    g_return_if_fail (SLOPE_IS_ITEM (self));

    m = SLOPE_ITEM_GET_PRIVATE (self);

    slope_tree_destroy (SLOPE_TREE (m), tree_cleanup);
}

/* slope/item.c */
