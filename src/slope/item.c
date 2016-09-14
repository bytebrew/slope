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
#include <slope/scale_p.h>


typedef struct
_SlopeItemPrivate
{
    SlopeFigure *figure;
    SlopeScale *scale;
    gboolean managed;
    gboolean visible;
}
SlopeItemPrivate;


#define SLOPE_ITEM_GET_CLASS(obj) \
    (SLOPE_ITEM_CLASS(G_OBJECT_GET_CLASS(obj)))

#define SLOPE_ITEM_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_ITEM_TYPE, SlopeItemPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeItem, slope_item, G_TYPE_OBJECT)


static void
slope_item_class_init (SlopeItemClass *klass)
{
    SLOPE_UNUSED(klass)
    /*
    // GObjectClass *object_klass = G_OBJECT_CLASS(klass);
    // TODO
    // object_klass->finalize = _scale_finalize;
    // klass->draw = _item_draw;
    */
}


static void
slope_item_init (SlopeItem *self)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);

    priv->figure = NULL;
    priv->scale = NULL;
    priv->managed = TRUE;
    priv->visible = TRUE;
}


void _item_set_scale (SlopeItem *self, SlopeScale *scale)
{
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);

    // TODO is has children
    priv->scale = scale;
    priv->figure = slope_scale_get_figure(scale);
}



gboolean slope_item_get_is_managed (SlopeItem *self)
{
    if (self != NULL) {
        return SLOPE_ITEM_GET_PRIVATE(self)->managed;
    }
    return FALSE;
}


void slope_item_set_is_managed (SlopeItem *self, gboolean managed)
{
    SLOPE_ITEM_GET_PRIVATE(self)->managed = managed;
}


gboolean slope_item_get_is_visible (SlopeItem *self)
{
    if (self != NULL) {
        return SLOPE_ITEM_GET_PRIVATE(self)->visible;
    }
    return FALSE;
}


void slope_item_set_is_visible (SlopeItem *self, gboolean visible)
{
    SLOPE_ITEM_GET_PRIVATE(self)->visible = visible;
}


void _item_draw (SlopeItem *self, cairo_t *cr)
{
    SLOPE_ITEM_GET_CLASS(self)->draw(self, cr);
}


void slope_item_get_figure_rect (SlopeItem *self, SlopeRect *rect)
{
    SLOPE_ITEM_GET_CLASS(self)->get_figure_rect(self, rect);
}


void slope_item_get_data_rect (SlopeItem *self, SlopeRect *rect)
{
    SLOPE_ITEM_GET_CLASS(self)->get_data_rect(self, rect);
}

/* slope/item.c */
