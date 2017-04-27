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

#include <slope/item_p.h>
#include <slope/scale_p.h>


typedef struct
_SlopeItemPrivate
{
    SlopeFigure *figure;
    SlopeScale *scale;
    SlopeView *view;
    char *name;
    gboolean managed;
    gboolean visible;
}
SlopeItemPrivate;



#define SLOPE_ITEM_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_ITEM_TYPE, SlopeItemPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeItem, slope_item, G_TYPE_OBJECT)


static void _item_finalize (GObject *self);
static void _item_draw_thumb_impl (SlopeItem *self, cairo_t *cr, const SlopePoint *pos);


static void
slope_item_class_init (SlopeItemClass *klass) {
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);
    object_klass->finalize = _item_finalize;
    klass->mouse_event = _item_mouse_event_impl;
    klass->draw_thumb = _item_draw_thumb_impl;
}

static void
slope_item_init (SlopeItem *self) {
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    priv->figure = NULL;
    priv->scale = NULL;
    priv->view = NULL;
    priv->name = NULL;
    priv->managed = TRUE;
    priv->visible = TRUE;
}

static void
_item_finalize (GObject *self) {
    /* release the name's memory */
    slope_item_set_name(SLOPE_ITEM(self), NULL);
    G_OBJECT_CLASS(slope_item_parent_class)->finalize(self);
}

void _item_set_scale (SlopeItem *self, SlopeScale *scale) {
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    /* TODO if it has children */
    priv->scale = scale;
    priv->figure =(scale != NULL) ?
        slope_scale_get_figure(scale) : NULL;
    priv->view = (priv->figure != NULL) ?
        slope_figure_get_view(priv->figure) : NULL;
}

void slope_item_detach (SlopeItem *self) {
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    if (priv->scale != NULL) {
        slope_scale_remove_item(priv->scale, self);
        priv->scale = NULL;
        priv->figure = NULL;
        priv->view = NULL;
    }
}

void _item_handle_mouse_event (SlopeItem *self, SlopeMouseEvent *event) {
    SLOPE_ITEM_GET_CLASS(self)->mouse_event(self, event);
}

void _item_mouse_event_impl (SlopeItem *self, SlopeMouseEvent *event) {
    /* provides a place holder "do nothing" implementation */
    SLOPE_UNUSED(self);
    SLOPE_UNUSED(event);
    /* pass */
}

void _item_draw (SlopeItem *self, cairo_t *cr) {
    SLOPE_ITEM_GET_CLASS(self)->draw(self, cr);
}

void _item_draw_thumb (SlopeItem *self, cairo_t *cr, const SlopePoint *pos) {
    SLOPE_ITEM_GET_CLASS(self)->draw_thumb(self, cr, pos);
}

static void
_item_draw_thumb_impl (SlopeItem *self, cairo_t *cr, const SlopePoint *pos) {
    /* provides a place holder "do nothing" implementation */
    SLOPE_UNUSED(self);
    SLOPE_UNUSED(cr);
    SLOPE_UNUSED(pos);
    /* pass */
}

void slope_item_get_figure_rect (SlopeItem *self, SlopeRect *rect) {
    SLOPE_ITEM_GET_CLASS(self)->get_figure_rect(self, rect);
}

void slope_item_get_data_rect (SlopeItem *self, SlopeRect *rect) {
    SLOPE_ITEM_GET_CLASS(self)->get_data_rect(self, rect);
}

gboolean slope_item_get_is_managed (SlopeItem *self) {
    return SLOPE_ITEM_GET_PRIVATE(self)->managed;
}

void slope_item_set_is_managed (SlopeItem *self, gboolean managed) {
    SLOPE_ITEM_GET_PRIVATE(self)->managed = managed;
}

gboolean slope_item_get_is_visible (SlopeItem *self) {
    return SLOPE_ITEM_GET_PRIVATE(self)->visible;
}

void slope_item_set_is_visible (SlopeItem *self, gboolean visible) {
    SLOPE_ITEM_GET_PRIVATE(self)->visible = visible;
}

char* slope_item_get_name (SlopeItem *self) {
    return SLOPE_ITEM_GET_PRIVATE(self)->name;
}

void slope_item_set_name (SlopeItem *self, const char *name) {
    SlopeItemPrivate *priv = SLOPE_ITEM_GET_PRIVATE(self);
    if (priv->name != NULL) {
        g_free(priv->name);
    }
    if (name != NULL) {
        priv->name = g_strdup(name);
    } else {
        priv->name = NULL;
    }
}

SlopeScale* slope_item_get_scale (SlopeItem *self) {
    return SLOPE_ITEM_GET_PRIVATE(self)->scale;
}

SlopeFigure* slope_item_get_figure (SlopeItem *self) {
    return SLOPE_ITEM_GET_PRIVATE(self)->figure;
}

SlopeView* slope_item_get_view (SlopeItem *self) {
    return SLOPE_ITEM_GET_PRIVATE(self)->view;
}

/* slope/item.c */
