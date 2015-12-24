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

#include <slope/scale_p.h>
#include <slope/item_p.h>
#include <stdlib.h>
#include <string.h>


void slope_scale_init (slope_scale_t *self)
{
    slope_scale_private_t *priv = SLOPE_SCALE_GET_PRIVATE(self);
    priv->item_list = slope_list_new();
    priv->visible = SLOPE_TRUE;
    priv->name = NULL;
}


void slope_scale_finalize (slope_scale_t *self)
{
    slope_scale_private_t *priv = SLOPE_SCALE_GET_PRIVATE(self);
    slope_list_destroy(priv->item_list);
    if (priv->name)
        SLOPE_FREE(priv->name);
    SLOPE_FREE(priv);
}


void slope_scale_destroy (slope_scale_t *self)
{
    if (self == NULL)
        return;
    SLOPE_SCALE_GET_CLASS(self)->finalize(self);
    SLOPE_FREE(self);
}


void slope_scale_add_item (slope_scale_t *self, slope_item_t *item)
{
    slope_scale_private_t *priv = SLOPE_SCALE_GET_PRIVATE(self);
    slope_list_append(priv->item_list, item);
    _slope_item_set_scale(item, self);
    slope_scale_update_for_item(self, item);
}


slope_list_t* slope_scale_get_item_list (const slope_scale_t *self)
{
    if (self == NULL)
        return NULL;
    return SLOPE_SCALE_GET_PRIVATE(self)->item_list;
}


slope_figure_t* slope_scale_get_figure (const slope_scale_t *self)
{
    if (self == NULL)
        return NULL;
    return SLOPE_SCALE_GET_PRIVATE(self)->figure;
}


slope_bool_t slope_scale_get_visible (const slope_scale_t *self)
{
    return SLOPE_SCALE_GET_PRIVATE(self)->visible;
}


void _slope_scale_draw (slope_scale_t *self, const slope_rect_t *rect, cairo_t *cr)
{
    SLOPE_SCALE_GET_CLASS(self)->draw(self, rect, cr);
}


void slope_scale_map (slope_scale_t *self, slope_point_t *figure_point,
                      const slope_point_t *data_point)
{
    SLOPE_SCALE_GET_CLASS(self)->map(self, figure_point, data_point);
}


void slope_scale_get_data_rect (const slope_scale_t *self, slope_rect_t *rect)
{
    SLOPE_SCALE_GET_CLASS(self)->get_data_rect(self, rect);
}


void slope_scale_get_figure_rect (const slope_scale_t *self, slope_rect_t *rect)
{
    SLOPE_SCALE_GET_CLASS(self)->get_figure_rect(self, rect);
}


void slope_scale_update (slope_scale_t *self)
{
    SLOPE_SCALE_GET_CLASS(self)->update(self);
}


void slope_scale_update_for_item (slope_scale_t *self, slope_item_t *item)
{
    SLOPE_SCALE_GET_CLASS(self)->update_for_item(self, item);
}


void _slope_scale_set_figure (slope_scale_t *self, slope_figure_t *figure)
{
    SLOPE_SCALE_GET_PRIVATE(self)->figure = figure;
}


void slope_scale_set_color_scheme (slope_scale_t *self, slope_color_t background,
                                   slope_color_t foreground, slope_color_t extra_color)
{
    SLOPE_SCALE_GET_CLASS(self)->set_color_scheme(self, background, foreground, extra_color);
}

/* slope/scale.c */
