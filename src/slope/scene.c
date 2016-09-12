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

#include <slope/scene_p.h>


typedef struct
_SlopeScenePrivate
{
    SlopeView *view;
    GList *item_list;
    SlopeColor background_color;
}
SlopeScenePrivate;


#define SLOPE_SCENE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_SCENE_TYPE, SlopeScenePrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeScene,
    slope_scene,
    G_TYPE_OBJECT)


static void _scene_add_item (SlopeScene *self, SlopeItem *item);
static void _scene_draw (SlopeScene *self, const SlopeRect *rect, cairo_t *cr);
static void _clear_item_list (gpointer data);
static void slope_scene_class_init (SlopeSceneClass *klass);
static void slope_scene_init (SlopeScene *self);
static void _scene_finalize (GObject *self);



static void
slope_scene_class_init (SlopeSceneClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->finalize = _scene_finalize;

    klass->add_item = _scene_add_item;
    klass->draw = _scene_draw;
}


static void
slope_scene_init (SlopeScene *self)
{
    SlopeScenePrivate *priv = SLOPE_SCENE_GET_PRIVATE(self);

    priv->view = NULL;
    priv->item_list = NULL;
    priv->background_color = SLOPE_WHITE;
}


static
void _scene_finalize (GObject *self)
{
    SlopeScenePrivate *priv = SLOPE_SCENE_GET_PRIVATE(self);
    GObjectClass *parent_class = g_type_class_peek_parent(G_OBJECT_GET_CLASS(self));

    if (priv->item_list != NULL) {
        g_list_free_full(priv->item_list, _clear_item_list);
    }

    G_OBJECT_CLASS(parent_class)->finalize(self);
}


SlopeScene* slope_scene_new ()
{
    SlopeScene *self = SLOPE_SCENE(g_object_new(SLOPE_SCENE_TYPE, NULL));

    return self;
}


GList* slope_scene_get_item_list (SlopeScene *self)
{
    if (self != NULL) {
        return SLOPE_SCENE_GET_PRIVATE(self)->item_list;
    }
    return NULL;
}


void slope_scene_add_item (SlopeScene *self, SlopeItem *item)
{
    SLOPE_SCENE_GET_CLASS(self)->add_item(self, item);
}


SlopeColor slope_scene_get_background_color (SlopeScene *self)
{
    if (self != NULL) {
        return SLOPE_SCENE_GET_PRIVATE(self)->background_color;
    }
    return SLOPE_COLOR_NULL;
}


void slope_scene_set_background_color (SlopeScene *self, SlopeColor color)
{
    SLOPE_SCENE_GET_PRIVATE(self)->background_color = color;
}


void slope_scene_draw (SlopeScene *self, const SlopeRect *rect, cairo_t *cr)
{
    SLOPE_SCENE_GET_CLASS(self)->draw(self, rect, cr);
}


static
void _scene_add_item (SlopeScene *self, SlopeItem *item)
{
    SlopeScenePrivate *priv = SLOPE_SCENE_GET_PRIVATE(self);

    g_return_if_fail(item != NULL);

    priv->item_list = g_list_append(priv->item_list, item);
}


static
void _scene_draw (SlopeScene *self, const SlopeRect *rect, cairo_t *cr)
{
    SlopeScenePrivate *priv = SLOPE_SCENE_GET_PRIVATE(self);

    /* save cr's state and clip tho the scene's rectangle,
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


void _scene_set_view (SlopeScene *self, SlopeView *view)
{
    SLOPE_SCENE_GET_PRIVATE(self)->view = view;
}


SlopeView* slope_scene_get_view (SlopeScene *self)
{
    if (self != NULL) {
        return SLOPE_SCENE_GET_PRIVATE(self)->view;
    }
    return NULL;
}


void _scene_mouse_event (SlopeScene *self, const SlopeMouseEvent *event)
{
    /* TODO */
}


void slope_scene_write_to_png (SlopeScene *self, const char *filename,
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

    slope_scene_draw(self, &rect, cr);
    cairo_surface_write_to_png(image, filename);

    cairo_surface_destroy(image);
    cairo_destroy(cr);
}

/* slope/scene.c */
