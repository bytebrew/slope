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

#include <slope/xyscale_p.h>
#include <slope/figure.h>
#include <slope/item.h>


typedef struct
_SlopeXyScalePrivate
{
    double left_margin, right_margin;
    double top_margin, bottom_margin;

    double horiz_pad, vertical_pad;

    double fig_x_min, fig_x_max;
    double fig_y_min, fig_y_max;
    double fig_width, fig_height;

    double dat_x_min, dat_x_max;
    double dat_y_min, dat_y_max;
    double dat_width, dat_height;
}
SlopeXyScalePrivate;


#define SLOPE_XYSCALE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_XYSCALE_TYPE, SlopeXyScalePrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeXyScale, slope_xyscale, SLOPE_SCALE_TYPE)



static void _xyscale_finalize (GObject *self);
static void _xyscale_draw (SlopeScale *self, const SlopeRect *rect, cairo_t *cr);
static void _xyscale_map (SlopeScale *self, SlopePoint *res, const SlopePoint *src);
static void _xyscale_unmap (SlopeScale *self, SlopePoint *res, const SlopePoint *src);
static void _xyscale_rescale (SlopeScale *self);
static void _xyscale_get_figure_rect (SlopeScale *self, SlopeRect *rect);
static void _xyscale_get_data_rect (SlopeScale *self, SlopeRect *rect);



static void
slope_xyscale_class_init (SlopeXyScaleClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);
    SlopeScaleClass *scale_klass = SLOPE_SCALE_CLASS(klass);

    object_klass->finalize = _xyscale_finalize;

    scale_klass->draw = _xyscale_draw;
    scale_klass->map = _xyscale_map;
    scale_klass->unmap = _xyscale_unmap;
    scale_klass->rescale = _xyscale_rescale;
    scale_klass->get_data_rect = _xyscale_get_data_rect;
    scale_klass->get_figure_rect = _xyscale_get_figure_rect;
}


static void
slope_xyscale_init (SlopeXyScale *self)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);

    priv->left_margin = 20.0;
    priv->right_margin = 20.0;
    priv->top_margin = 20.0;
    priv->bottom_margin = 20.0;

    priv->horiz_pad = 0.03;
    priv->vertical_pad = 0.03;

    slope_scale_rescale(SLOPE_SCALE(self));
}


static
void _xyscale_finalize (GObject *self)
{
    SLOPE_UNUSED(self);
    // TODO
}


SlopeScale* slope_xyscale_new ()
{
    SlopeXyScale *self = SLOPE_XYSCALE(g_object_new(SLOPE_XYSCALE_TYPE, NULL));

    return SLOPE_SCALE(self);
}


static
void _xyscale_draw (SlopeScale *self, const SlopeRect *rect, cairo_t *cr)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);

    priv->fig_x_min = rect->x + priv->left_margin;
    priv->fig_x_max = rect->x + rect->width - priv->right_margin;
    priv->fig_width = priv->fig_x_max - priv->fig_x_min;

    priv->fig_y_min = rect->y + priv->top_margin;
    priv->fig_y_max = rect->y + rect->height - priv->bottom_margin;
    priv->fig_height = priv->fig_y_max - priv->fig_y_min;

    cairo_save(cr);
    cairo_new_path(cr);
    cairo_rectangle(cr, priv->fig_x_min, priv->fig_y_min,
                    priv->fig_width, priv->fig_height);
    cairo_clip(cr);

    SLOPE_SCALE_CLASS(
        g_type_class_peek_parent(
            SLOPE_XYSCALE_GET_CLASS(self)))
                ->draw(self, rect, cr);

    cairo_restore(cr);
}


static
void _xyscale_map (SlopeScale *self, SlopePoint *res, const SlopePoint *src)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    double tmp;

    tmp = (src->x - priv->dat_x_min) / priv->dat_width;
    res->x = priv->fig_x_min + tmp * priv->fig_width;

    tmp = (src->y - priv->dat_y_min) / priv->dat_height;
    res->x = priv->fig_y_max - tmp * priv->fig_height;
}


static
void _xyscale_unmap (SlopeScale *self, SlopePoint *res, const SlopePoint *src)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    double tmp;

    tmp = (src->x - priv->fig_x_min) / priv->fig_width;
    res->x = priv->dat_x_min + tmp * priv->dat_width;

    tmp = (priv->dat_y_max - src->y) / priv->fig_height;
    res->x = priv->dat_y_min + tmp * priv->dat_height;
}


static
void _xyscale_rescale (SlopeScale *self)
{
    SlopeXyScalePrivate *priv;
    GList *iter, *list;
    SlopeItem *item;
    SlopeRect item_rect;
    double padding;

    priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    list = slope_scale_get_item_list(self);

    if (list == NULL) {
        priv->dat_x_min = 0.0;
        priv->dat_x_max = 1.0;
        priv->dat_y_min = 0.0;
        priv->dat_y_max = 1.0;
        return;
    }

    iter = list;
    item = SLOPE_ITEM(iter->data);
    slope_item_get_data_rect(item, &item_rect);

    /* initialize to the first item to avoid junk values */
    priv->dat_x_min = item_rect.x;
    priv->dat_x_max = item_rect.x + item_rect.width;
    priv->dat_y_min = item_rect.y;
    priv->dat_y_max = item_rect.y + item_rect.height;

    while (iter != NULL) {
        item = SLOPE_ITEM(iter->data);
        slope_item_get_data_rect(item, &item_rect);
        iter = iter->next;

        if (item_rect.x < priv->dat_x_min)
            priv->dat_x_min = item_rect.x;
        if ((item_rect.x+item_rect.width) > priv->dat_x_max)
            priv->dat_x_max = (item_rect.x+item_rect.width);
        if (item_rect.y < priv->dat_y_min)
            priv->dat_y_min = item_rect.y;
        if ((item_rect.y+item_rect.height) > priv->dat_y_max)
            priv->dat_y_max = (item_rect.y+item_rect.height);
    }

    /* evaluate width and height of the data space taking
     * int account the padding */
    priv->dat_width = priv->dat_x_max - priv->dat_x_min;
    padding = priv->dat_width * priv->horiz_pad;
    priv->dat_x_min -= padding;
    priv->dat_x_max += padding;
    priv->dat_width += 2.0*padding;

    priv->dat_height = priv->dat_y_max - priv->dat_y_min;
    padding = priv->dat_height * priv->vertical_pad;
    priv->dat_y_min -= padding;
    priv->dat_y_max += padding;
    priv->dat_height += 2.0*padding;
}


static
void _xyscale_get_figure_rect (SlopeScale *self, SlopeRect *rect)
{
    SlopeXyScalePrivate *priv;

    if (self == NULL) {
        return;
    }

    priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    rect->x = priv->fig_x_min;
    rect->y = priv->fig_y_min;
    rect->width = priv->fig_width;
    rect->height = priv->fig_height;
}


static
void _xyscale_get_data_rect (SlopeScale *self, SlopeRect *rect)
{
    SlopeXyScalePrivate *priv;

    if (self == NULL) {
        return;
    }

    priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    rect->x = priv->dat_x_min;
    rect->y = priv->dat_y_min;
    rect->width = priv->dat_width;
    rect->height = priv->dat_height;
}

/* slope/xyscale.c */
