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

#include <slope/figure_p.h>


typedef struct
_SlopeFigurePrivate
{
    SlopeView *view;
    GList *item_list;
    SlopeColor background_color;
}
SlopeFigurePrivate;


#define SLOPE_FIGURE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_FIGURE_TYPE, SlopeFigurePrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeFigure,
    slope_figure,
    G_TYPE_OBJECT)


static void _figure_add_item (SlopeFigure *self, SlopeItem *item);
static void _figure_draw (SlopeFigure *self, const SlopeRect *rect, cairo_t *cr);
static void _clear_item_list (gpointer data);
static void slope_figure_class_init (SlopeFigureClass *klass);
static void slope_figure_init (SlopeFigure *self);
static void _figure_finalize (GObject *self);



static void
slope_figure_class_init (SlopeFigureClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->finalize = _figure_finalize;

    klass->add_item = _figure_add_item;
    klass->draw = _figure_draw;
}


static void
slope_figure_init (SlopeFigure *self)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);

    priv->background_color = SLOPE_WHITE;
    priv->item_list = NULL;
}


static
void _figure_finalize (GObject *self)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);
    GObjectClass *parent_class = g_type_class_peek_parent(G_OBJECT_GET_CLASS(self));

    if (priv->item_list != NULL) {
        g_list_free_full(priv->item_list, _clear_item_list);
    }

    G_OBJECT_CLASS(parent_class)->finalize(self);
}


SlopeFigure* slope_figure_new ()
{
    SlopeFigure *self = SLOPE_FIGURE(g_object_new(SLOPE_FIGURE_TYPE, NULL));

    return self;
}


GList* slope_figure_get_item_list (SlopeFigure *self)
{
    if (self != NULL) {
        return SLOPE_FIGURE_GET_PRIVATE(self)->item_list;
    }
    return NULL;
}


void slope_figure_add_item (SlopeFigure *self, SlopeItem *item)
{
    SLOPE_FIGURE_GET_CLASS(self)->add_item(self, item);
}


SlopeColor slope_figure_get_background_color (SlopeFigure *self)
{
    if (self != NULL) {
        return SLOPE_FIGURE_GET_PRIVATE(self)->background_color;
    }
    return SLOPE_COLOR_NULL;
}


void slope_figure_set_background_color (SlopeFigure *self, SlopeColor color)
{
    SLOPE_FIGURE_GET_PRIVATE(self)->background_color = color;
}


void slope_figure_draw (SlopeFigure *self, const SlopeRect *rect, cairo_t *cr)
{
    SLOPE_FIGURE_GET_CLASS(self)->draw(self, rect, cr);
}


static
void _figure_add_item (SlopeFigure *self, SlopeItem *item)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);

    g_return_if_fail(item != NULL);

    priv->item_list = g_list_append(priv->item_list, item);
}


static
void _figure_draw (SlopeFigure *self, const SlopeRect *rect, cairo_t *cr)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);

    /* save cr's state and clip tho the figure's rectangle,
       fill the background if required */
    cairo_save(cr);
    cairo_new_path(cr);
    slope_cairo_rect(cr, rect);
    if (!SLOPE_COLOR_IS_NULL(priv->background_color)) {
        slope_cairo_set_color(cr, priv->background_color);
        cairo_fill_preserve(cr);
    }
    cairo_clip(cr);


    /* TODO */


    /* give back cr in the same state as we received it */
    cairo_restore(cr);
}


static
void _clear_item_list (gpointer data)
{
    /* TODO */
}


void _figure_set_view (SlopeFigure *self, SlopeView *view)
{
    SLOPE_FIGURE_GET_PRIVATE(self)->view = view;
}


SlopeView* slope_figure_get_view (SlopeFigure *self)
{
    if (self != NULL) {
        return SLOPE_FIGURE_GET_PRIVATE(self)->view;
    }
    return NULL;
}


void _figure_mouse_event (SlopeFigure *self, const SlopeMouseEvent *event)
{
    /* TODO */
}


void slope_figure_write_to_png (SlopeFigure *self, const char *filename,
                               int width, int height)
{
    cairo_surface_t *image;
    cairo_t *cr;
    SlopeRect rect;

    if (self == NULL || filename == NULL || width <= 0 || height <= 0) {
        return;
    }

    image = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(image);

    rect.x = 0.0;
    rect.y = 0.0;
    rect.width = width;
    rect.height = height;

    slope_figure_draw(self, &rect, cr);
    cairo_surface_write_to_png(image, filename);

    cairo_surface_destroy(image);
    cairo_destroy(cr);
}

/* slope/figure.c */
