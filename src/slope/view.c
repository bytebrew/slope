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

#include <slope/view.h>


typedef struct
_SlopeViewPrivate
{
    SlopeScene *scene;
}
SlopeViewPrivate;


#define SLOPE_VIEW_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_VIEW_TYPE, SlopeViewPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeView,
    slope_view,
    GTK_TYPE_DRAWING_AREA)


static void slope_view_class_init (SlopeViewClass *klass);
static void slope_view_init (SlopeView *self);
static void _view_finalize (GObject *self);
static void _view_set_scene (SlopeView *self, SlopeScene *scene);
static gboolean _view_draw (GtkWidget *self, cairo_t *cr, gpointer data);



static void
slope_view_class_init (SlopeViewClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->finalize = _view_finalize;

    klass->set_scene = _view_set_scene;
}


static void
slope_view_init (SlopeView *self)
{
    GtkWidget *gtk_widget = GTK_WIDGET(self);
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);

    priv->scene = NULL;

    gtk_widget_set_size_request(gtk_widget, 250, 250);
    g_signal_connect(G_OBJECT(self), "draw", G_CALLBACK(_view_draw), NULL);
}


static
void _view_finalize (GObject *self)
{
    /* TODO */
}


GtkWidget* slope_view_new ()
{
    GtkWidget *self = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));

    return self;
}


GtkWidget* slope_view_new_with_scene (SlopeScene *scene)
{
    GtkWidget *self = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));

    slope_view_set_scene(self, scene);

    return self;
}


static
void _view_set_scene (SlopeView *self, SlopeScene *scene)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);

    priv->scene = scene;
}


void slope_view_set_scene (SlopeView *self, SlopeScene *scene)
{
    SLOPE_VIEW_GET_CLASS(self)->set_scene(self, scene);
}


SlopeScene* slope_view_get_scene (SlopeScene *self)
{
    if (self != NULL) {
        SLOPE_VIEW_GET_PRIVATE(self)->scene;
    }
    return NULL;
}


static
gboolean _view_draw (GtkWidget *self, cairo_t *cr, gpointer data)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
    GtkAllocation allocation;
    SlopeRect rect;

    if (priv->scene == NULL) {
        return TRUE;
    }

    gtk_widget_get_allocation(self, &allocation);
    rect.x = 0.0;
    rect.y = 0.0;
    rect.width = allocation.width;
    rect.height = allocation.height;

    slope_scene_draw(priv->scene, &rect, cr);
    return TRUE;
}

/* slope/view.c */
