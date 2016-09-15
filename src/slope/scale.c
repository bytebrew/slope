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

#include <slope/scale_p.h>
#include <slope/item_p.h>

typedef struct
_SlopeScalePrivate
{
    SlopeFigure *figure;
    GList *item_list;
    SlopeColor background_color;
    gboolean managed;
    gboolean visible;

    char *name;
    gboolean show_name;
    SlopeColor name_color;
    double name_top_padding;

    SlopeRect layout_rect;
}
SlopeScalePrivate;



#define SLOPE_SCALE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_SCALE_TYPE, SlopeScalePrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeScale, slope_scale, G_TYPE_OBJECT)


static void _scale_draw_impl (SlopeScale *self, const SlopeRect *rect, cairo_t *cr);
static void _scale_finalize (GObject *self);
static void _scale_add_item (SlopeScale *self, SlopeItem *item);
static void _scale_clear_item_list (gpointer data);



static void
slope_scale_class_init (SlopeScaleClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->finalize = _scale_finalize;

    klass->add_item = _scale_add_item;
    klass->draw = _scale_draw_impl;
}


static void
slope_scale_init (SlopeScale *self)
{
    SlopeScalePrivate *priv = SLOPE_SCALE_GET_PRIVATE(self);

    priv->figure = NULL;
    priv->item_list = NULL;
    priv->background_color = SLOPE_WHITE;
    priv->managed = TRUE;
    priv->visible = TRUE;

    priv->name = NULL;
    priv->show_name = FALSE;
    priv->name_color = SLOPE_BLACK;
    priv->name_top_padding = 0.0;

    priv->layout_rect.x = 0.0;
    priv->layout_rect.y = 0.0;
    priv->layout_rect.width = 1.0;
    priv->layout_rect.height = 1.0;
}


static
void _scale_finalize (GObject *self)
{
    GObjectClass *parent_class = g_type_class_peek_parent(G_OBJECT_GET_CLASS(self));
    SlopeScalePrivate *priv = SLOPE_SCALE_GET_PRIVATE(self);

    /* frees name safely */
    slope_scale_set_name(SLOPE_SCALE(self), NULL);

    if (priv->item_list != NULL) {
        g_list_free_full(priv->item_list, _scale_clear_item_list);
        priv->item_list = NULL;
    }

    G_OBJECT_CLASS(parent_class)->finalize(self);
}


static
void _scale_add_item (SlopeScale *self, SlopeItem *item)
{
    SlopeScalePrivate *priv = SLOPE_SCALE_GET_PRIVATE(self);

    if (item == NULL) {
        return;
    }

    priv->item_list = g_list_append(priv->item_list, item);
    _item_set_scale(item, self);
    slope_scale_rescale(self);
}


void _scale_draw_impl (SlopeScale *self, const SlopeRect *rect, cairo_t *cr)
{
    SlopeScalePrivate *priv = SLOPE_SCALE_GET_PRIVATE(self);
    GList *iter;

    if (!SLOPE_COLOR_IS_NULL(priv->background_color)) {
        cairo_save(cr);
        cairo_new_path(cr);
        slope_cairo_rect(cr, rect);
        slope_cairo_set_color(cr, priv->background_color);
        cairo_fill(cr);
        cairo_restore(cr);
    }

    iter = priv->item_list;
    while (iter != NULL) {
        SlopeItem *item = SLOPE_ITEM(iter->data);

        if (slope_item_get_is_visible(item) == TRUE) {
            _item_draw(item, cr);
        }

        iter = iter->next;
    }

    if (priv->name != NULL && priv->show_name == TRUE) {
        cairo_text_extents_t txt_ext;
        cairo_text_extents(cr, priv->name, &txt_ext);
        slope_cairo_set_color(cr, priv->name_color);
        slope_cairo_text(cr,
            rect->x + (rect->width - txt_ext.width)*0.5,
            rect->y + txt_ext.height*1.64 + priv->name_top_padding,
            priv->name);
        cairo_stroke(cr);
    }
}


void _scale_set_figure (SlopeScale *self, SlopeFigure *figure)
{
    SlopeScalePrivate *priv = SLOPE_SCALE_GET_PRIVATE(self);
    GList *iter;


    if (priv->figure == figure) {
        return;
    }

    priv->figure = figure;

    iter = priv->item_list;
    while (iter != NULL) {
        SlopeItem *item = SLOPE_ITEM(iter->data);

        /* update children scale and figure infos */
        _item_set_scale(item, self);

        iter = iter->next;
    }
}


static
void _scale_clear_item_list (gpointer data)
{
    if (slope_item_get_is_managed(SLOPE_ITEM(data)) == TRUE) {
        g_object_unref(G_OBJECT(data));
    }
}


SlopeFigure* slope_scale_get_figure (SlopeScale *self)
{
    if (self != NULL) {
        return SLOPE_SCALE_GET_PRIVATE(self)->figure;
    }
    return NULL;
}


gboolean slope_scale_get_is_managed (SlopeScale *self)
{
    if (self != NULL) {
        return SLOPE_SCALE_GET_PRIVATE(self)->managed;
    }
    return FALSE;
}


void slope_scale_set_is_managed (SlopeScale *self, gboolean managed)
{
    SLOPE_SCALE_GET_PRIVATE(self)->managed = managed;
}


gboolean slope_scale_get_is_visible (SlopeScale *self)
{
    if (self != NULL) {
        return SLOPE_SCALE_GET_PRIVATE(self)->visible;
    }
    return FALSE;
}


void slope_scale_set_is_visible (SlopeScale *self, gboolean visible)
{
    SLOPE_SCALE_GET_PRIVATE(self)->visible = visible;
}


void slope_scale_get_layout_rect (SlopeScale *self, SlopeRect *rect)
{
    SlopeScalePrivate *priv;

    if (self == NULL) {
        return;
    }

    priv = SLOPE_SCALE_GET_PRIVATE(self);
    rect->x = priv->layout_rect.x;
    rect->y = priv->layout_rect.y;
    rect->width = priv->layout_rect.width;
    rect->height = priv->layout_rect.height;
}


void slope_scale_set_layout_rect (SlopeScale *self, double x, double y, double w, double h)
{
    SlopeScalePrivate *priv = SLOPE_SCALE_GET_PRIVATE(self);

    priv->layout_rect.x = x;
    priv->layout_rect.y = y;
    priv->layout_rect.width = w;
    priv->layout_rect.height = h;
}


SlopeColor slope_scale_get_background_color (SlopeScale *self)
{
    if (self != NULL) {
        return SLOPE_SCALE_GET_PRIVATE(self)->background_color;
    }
    return SLOPE_COLOR_NULL;
}


void slope_scale_set_background_color (SlopeScale *self, SlopeColor color)
{
    SLOPE_SCALE_GET_PRIVATE(self)->background_color = color;
}


void _scale_draw (SlopeScale *self, const SlopeRect *rect, cairo_t *cr)
{
    SLOPE_SCALE_GET_CLASS(self)->draw(self, rect, cr);
}


void slope_scale_get_figure_rect (SlopeScale *self, SlopeRect *rect)
{
    SLOPE_SCALE_GET_CLASS(self)->get_figure_rect(self, rect);
}


void slope_scale_get_data_rect (SlopeScale *self, SlopeRect *rect)
{
    SLOPE_SCALE_GET_CLASS(self)->get_data_rect(self, rect);
}


GList* slope_scale_get_item_list (SlopeScale *self)
{
    return SLOPE_SCALE_GET_PRIVATE(self)->item_list;
}


void slope_scale_set_name (SlopeScale *self, const char *name)
{
    SlopeScalePrivate *priv = SLOPE_SCALE_GET_PRIVATE(self);

    if (priv->name != NULL) {
        g_free(priv->name);
    }

    if (name != NULL) {
        priv->name = g_strdup(name);
        priv->show_name = TRUE;
    } else {
        priv->name = NULL;
    }
}


char* slope_scale_get_name (SlopeScale *self)
{
    return SLOPE_SCALE_GET_PRIVATE(self)->name;
}


void slope_scale_set_show_name (SlopeScale *self, gboolean show)
{
    SLOPE_SCALE_GET_PRIVATE(self)->show_name = show;
}


gboolean slope_scale_get_show_name (SlopeScale *self)
{
    return SLOPE_SCALE_GET_PRIVATE(self)->show_name;
}


void slope_scale_set_name_top_padding (SlopeScale *self, double padding)
{
    SLOPE_SCALE_GET_PRIVATE(self)->name_top_padding = padding;
}


void slope_scale_add_item (SlopeScale *self, SlopeItem *item)
{
    SLOPE_SCALE_GET_CLASS(self)->add_item(self, item);
}


void slope_scale_map (SlopeScale *self, SlopePoint *res, const SlopePoint *src)
{
    SLOPE_SCALE_GET_CLASS(self)->map(self, res, src);
}


void slope_scale_unmap (SlopeScale *self, SlopePoint *res, const SlopePoint *src)
{
    SLOPE_SCALE_GET_CLASS(self)->unmap(self, res, src);
}


void slope_scale_rescale (SlopeScale *self)
{
    SLOPE_SCALE_GET_CLASS(self)->rescale(self);
}

/* slope/scale.c */
