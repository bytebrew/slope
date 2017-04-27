/*
 * Copyright (C) 2017  Elvis Teixeira
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

#include <slope/legend.h>
#include <slope/scale.h>
#include <slope/item_p.h>

typedef struct
_SlopeLegendPrivate
{
    guint32 orientation;
    double x, y;
    double entry_height;
    double rect_stroke_width;
    SlopeColor rect_stroke_color;
    SlopeColor rect_fill_color;
    SlopeColor text_color;
    gboolean rect_antialias;
    SlopePoint p1, p2;
    SlopeCorner anchor;
    GList *items;
}
SlopeLegendPrivate;


#define SLOPE_LEGEND_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_LEGEND_TYPE, SlopeLegendPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeLegend, slope_legend, SLOPE_ITEM_TYPE)


static void _legend_finalize (GObject *self);
static void _legend_draw (SlopeItem *self, cairo_t *cr);
static void _legend_get_figure_rect (SlopeItem *self, SlopeRect *rect);
static void _legend_get_data_rect (SlopeItem *self, SlopeRect *rect);
static void _legend_evaluate_rect (SlopeItem *self, cairo_t *cr);
static void _legend_draw_rect (SlopeItem *self, cairo_t *cr);
static void _legend_draw_thumbs (SlopeItem *self, cairo_t *cr);


static void
slope_legend_class_init (SlopeLegendClass *klass) {
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);
    SlopeItemClass *item_klass = SLOPE_ITEM_CLASS(klass);
    object_klass->finalize = _legend_finalize;
    object_klass->finalize = _legend_finalize;
    item_klass->draw = _legend_draw;
    item_klass->get_data_rect = _legend_get_data_rect;
    item_klass->get_figure_rect = _legend_get_figure_rect;
}

static void
slope_legend_init (SlopeLegend *self) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    priv->orientation = SLOPE_HORIZONTAL;
    priv->rect_fill_color = SLOPE_WHITE;
    priv->rect_stroke_color = SLOPE_BLACK;
    priv->text_color = SLOPE_BLACK;
    priv->rect_stroke_width = 1.0;
    priv->rect_antialias = FALSE;
    priv->items = NULL;
    priv->entry_height = 0.0;
    priv->anchor = SLOPE_TOPLEFT;
}

static void
_legend_finalize (GObject *self) {
    slope_legend_clear_items(SLOPE_LEGEND(self));
    G_OBJECT_CLASS(slope_legend_parent_class)->finalize(self);
}

SlopeItem *slope_legend_new (SlopeOrientation orientation) {
    SlopeItem *self = SLOPE_ITEM(g_object_new(SLOPE_LEGEND_TYPE, NULL));
    slope_legend_set_orientation(SLOPE_LEGEND(self), orientation);
    return self;
}

static void
_legend_draw (SlopeItem *self, cairo_t *cr) {
    _legend_evaluate_rect(self, cr);
    _legend_draw_rect(self, cr);
    _legend_draw_thumbs(self, cr);
}

static void
_legend_evaluate_rect (SlopeItem *self, cairo_t *cr) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    SlopeScale *scale = slope_item_get_scale(self);
    /* top left point (in figure coordinates) is easy because
       it is given by the user. as the x and y coordinates
       in set_position */
    SlopePoint data_p1;
    data_p1.x = priv->x;
    data_p1.y = priv->y;
    if (scale != NULL) {
        /* if there is a scale, use it to transform the point */
        slope_scale_map(scale, &priv->p1, &data_p1);
    } else {
        /* if there's none, use the user provided position as
           the legend's figure position */
        priv->p1 = data_p1;
    }
    /* Bottom right (in figure coordinates) is tricky, because
       it depends on legend entries extents */
    double width = 0.0;
    double height = 0.0;
    cairo_text_extents_t txt_ext;
    GList *item_iter = priv->items;
    priv->entry_height = 0.0;
    while (item_iter != NULL) {
        SlopeItem *item = SLOPE_ITEM(item_iter->data);
        const char *item_name = slope_item_get_name(item);
        cairo_text_extents(cr, item_name, &txt_ext);
        if (txt_ext.height > priv->entry_height) {
            priv->entry_height = txt_ext.height;
        }
        if (priv->orientation == SLOPE_HORIZONTAL) {
            width += txt_ext.width + 50.0;
        } else {
            if (txt_ext.width > width) width = txt_ext.width;
            height += 1.0;
        }
        item_iter = item_iter->next;
    }
    /* TODO: break it in evaluate_extents, and evaluate_rect to
       use the anchor property */
    priv->entry_height += 10.0;
    if (priv->orientation == SLOPE_HORIZONTAL) {
        height = priv->entry_height + 10.0;
        width += 10.0;
    } else {
        height *= priv->entry_height;
        height += 10.0;
        width += 50.0;
    }
    priv->p2.x = priv->p1.x + width;
    priv->p2.y = priv->p1.y + height;
}

static void
_legend_draw_rect (SlopeItem *self, cairo_t *cr) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    gboolean should_stroke = !SLOPE_COLOR_IS_NULL(priv->rect_stroke_color);
    gboolean should_fill = !SLOPE_COLOR_IS_NULL(priv->rect_fill_color);
    if (should_fill || should_stroke) {
        cairo_new_path(cr);
        cairo_set_line_width(cr, priv->rect_stroke_width);
        slope_cairo_set_antialias(cr, priv->rect_antialias);
        cairo_rectangle(
            cr,
            priv->p1.x,
            priv->p1.y,
            priv->p2.x - priv->p1.x,
            priv->p2.y - priv->p1.y);
        if (should_fill && should_stroke) {
            slope_cairo_set_color(cr, priv->rect_fill_color);
            cairo_fill_preserve(cr);
            slope_cairo_set_color(cr, priv->rect_stroke_color);
            cairo_stroke(cr);
        } else if (should_fill) {
            slope_cairo_set_color(cr, priv->rect_fill_color);
            cairo_fill(cr);
        } else {
            slope_cairo_set_color(cr, priv->rect_stroke_color);
            cairo_stroke(cr);
        }
    }
}

static void
_legend_draw_thumbs (SlopeItem *self, cairo_t *cr) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    GList *item_iter = priv->items;
    SlopePoint pos;
    pos.x = priv->p1.x + 20.0;
    pos.y = priv->p1.y + 10.0 + priv->entry_height/2.0;
    while (item_iter != NULL) {
        SlopeItem *item = SLOPE_ITEM(item_iter->data);
        const char *item_name = slope_item_get_name(item);
        _item_draw_thumb(item, cr, &pos);
        if (priv->orientation == SLOPE_HORIZONTAL) {
            slope_cairo_set_color(cr, priv->text_color);
            slope_cairo_text(cr, pos.x + 20.0, pos.y, item_name);
            cairo_text_extents_t txt_ext;
            cairo_text_extents(cr, item_name, &txt_ext);
            pos.x += txt_ext.width + 50.0;
        } else {
            slope_cairo_set_color(cr, priv->text_color);
            slope_cairo_text(cr, pos.x + 20.0, pos.y, item_name);
            pos.y += priv->entry_height;
        }
        cairo_stroke(cr);
        item_iter = item_iter->next;
    }
}

static void
_legend_get_figure_rect (SlopeItem *self, SlopeRect *rect) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    rect->x = priv->p1.x;
    rect->y = priv->p1.y;
    rect->width = priv->p2.x - priv->p1.x;
    rect->height = priv->p2.y - priv->p1.y;
}

static void
_legend_get_data_rect (SlopeItem *self, SlopeRect *rect) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    SlopeScale *scale = slope_item_get_scale(self);
    if (scale == NULL) {
        rect->x = 0.0;
        rect->y = 0.0;
        rect->width = 0.0;
        rect->height = 0.0;
    } else {
        SlopePoint data_p2;
        slope_scale_unmap(scale, &data_p2, &priv->p2);
        rect->x = priv->x;
        rect->y = data_p2.y;
        rect->width = data_p2.x - priv->x;
        rect->height = priv->y - data_p2.y;
    }
}

void slope_legend_set_orientation (SlopeLegend *self,
                                   SlopeOrientation orientation) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    priv->orientation = orientation;
}

void slope_legend_set_anchor (SlopeLegend *self, SlopeCorner anchor) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    priv->anchor = anchor;
}

void slope_legend_set_position (SlopeLegend *self, double x, double y) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    priv->x = x;
    priv->y = y;
}

void slope_legend_add_item (SlopeLegend *self, SlopeItem *item) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    priv->items = g_list_append(priv->items, (gpointer) item);
}

void slope_legend_clear_items (SlopeLegend *self) {
    SlopeLegendPrivate *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    if (priv->items != NULL) {
        g_list_free(priv->items);
        priv->items = NULL;
    }
}

/* slope/legend.c */
