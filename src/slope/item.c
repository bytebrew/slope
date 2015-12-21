/*
 * Copyright (C) 2015  Elvis Teixeira
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
#include <stdlib.h>
#include <string.h>

void slope_item_init (slope_item_t *self)
{
    slope_item_private_t *priv = SLOPE_ITEM_GET_PRIVATE(self);
    priv->visible = SLOPE_TRUE;
    priv->has_thumb = SLOPE_TRUE;
    priv->scale = NULL;
    priv->name = NULL;
}


void slope_item_finalize (slope_item_t *self)
{
    slope_item_private_t *priv = SLOPE_ITEM_GET_PRIVATE(self);
    if (priv->name)
        SLOPE_FREE(priv->name);
    SLOPE_FREE(priv);
}


void slope_item_destroy (slope_item_t *self)
{
    if (self == NULL)
        return;
    SLOPE_ITEM_GET_CLASS(self)->finalize(self);
    SLOPE_FREE(self);
}


slope_bool_t slope_item_get_visible (const slope_item_t *self)
{
    if (self == NULL)
        return SLOPE_FALSE;
    return SLOPE_ITEM_GET_PRIVATE(self)->visible;
}


slope_bool_t slope_item_has_thumb (const slope_item_t *self)
{
    if (self == NULL)
        return SLOPE_FALSE;
    return SLOPE_ITEM_GET_PRIVATE(self)->has_thumb;
}


void _slope_item_draw (slope_item_t *self, cairo_t *cr)
{
    SLOPE_ITEM_GET_CLASS(self)->draw(self, cr);
}


void _slope_item_draw_thumb (slope_item_t *self, const slope_point_t *point, cairo_t *cr)
{
    SLOPE_ITEM_GET_CLASS(self)->draw_thumb(self, point, cr);
}


void _slope_item_draw_thumb_dummy_impl (slope_item_t *self, const slope_point_t *point, cairo_t *cr)
{
    /* DO NOTHING */
    (void) self;
    (void) point;
    (void) cr;
}


void slope_item_get_data_rect (const slope_item_t *self, slope_rect_t *rect)
{
    if (self == NULL)
        return;
    SLOPE_ITEM_GET_CLASS(self)->get_data_rect(self, rect);
}


void slope_item_get_figure_rect (const slope_item_t *self, slope_rect_t *rect)
{
    if (self == NULL)
        return;
    SLOPE_ITEM_GET_CLASS(self)->get_figure_rect(self, rect);
}


void _slope_item_set_scale (slope_item_t *self, slope_scale_t *scale)
{
    SLOPE_ITEM_GET_PRIVATE(self)->scale = scale;
}


const char* slope_item_get_name (const slope_item_t *self)
{
    if (self == NULL)
        return NULL;
    return SLOPE_ITEM_GET_PRIVATE(self)->name;
}


void slope_item_set_name (slope_item_t *self, const char *name)
{
    slope_item_private_t *priv;
    
    if (self == NULL)
        return;
    
    priv = SLOPE_ITEM_GET_PRIVATE(self);
    if (priv->name != NULL)
        SLOPE_FREE(priv->name);
    priv->name = strdup(name);
}

/* slope/item.c */
