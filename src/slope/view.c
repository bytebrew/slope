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
#include <slope/figure_p.h>


typedef struct
_SlopeViewPrivate
{
    SlopeFigure *figure;
    gboolean ownmem;
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
static void _view_set_figure (SlopeView *self, SlopeFigure *figure, gboolean ownmem);
static gboolean _view_on_draw (GtkWidget *self, cairo_t *cr, gpointer data);
static gboolean _view_on_button_press (GtkWidget *self, GdkEvent *event, gpointer data);



static void
slope_view_class_init (SlopeViewClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->finalize = _view_finalize;

    klass->set_figure = _view_set_figure;
}


static void
slope_view_init (SlopeView *self)
{
    GtkWidget *gtk_widget = GTK_WIDGET(self);
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);

    priv->figure = NULL;
    priv->ownmem = FALSE;

    gtk_widget_set_size_request(gtk_widget, 250, 250);
    gtk_widget_add_events(gtk_widget,
                GDK_EXPOSURE_MASK
                |GDK_BUTTON_MOTION_MASK
                |GDK_BUTTON_PRESS_MASK
                |GDK_BUTTON_RELEASE_MASK);

    g_signal_connect(G_OBJECT(self), "draw",
                     G_CALLBACK(_view_on_draw), NULL);
    g_signal_connect(G_OBJECT(self), "button-press-event",
                     G_CALLBACK(_view_on_button_press), NULL);
}


static
void _view_finalize (GObject *self)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
    GObjectClass *parent_class = g_type_class_peek_parent(G_OBJECT_GET_CLASS(self));

    if (priv->figure != NULL && priv->ownmem == TRUE) {
        g_object_unref(priv->figure);
    }

    G_OBJECT_CLASS(parent_class)->finalize(self);
}


GtkWidget* slope_view_new ()
{
    GtkWidget *self = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));

    return self;
}


GtkWidget* slope_view_new_with_figure (SlopeFigure *figure, gboolean ownmem)
{
    GtkWidget *self = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));

    slope_view_set_figure(SLOPE_VIEW(self), figure, ownmem);

    return self;
}


static
void _view_set_figure (SlopeView *self, SlopeFigure *figure, gboolean ownmem)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);

    priv->figure = figure;
    priv->ownmem = ownmem;
    _figure_set_view(figure, self);
}


void slope_view_set_figure (SlopeView *self, SlopeFigure *figure, gboolean ownmem)
{
    SLOPE_VIEW_GET_CLASS(self)->set_figure(self, figure, ownmem);
}


SlopeFigure* slope_view_get_figure (SlopeFigure *self)
{
    if (self != NULL) {
        return SLOPE_VIEW_GET_PRIVATE(self)->figure;
    }
    return NULL;
}


static
gboolean _view_on_draw (GtkWidget *self, cairo_t *cr, gpointer data)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
    GtkAllocation allocation;
    SlopeRect rect;
    SLOPE_UNUSED(data)

    if (priv->figure == NULL) {
        return TRUE;
    }

    gtk_widget_get_allocation(self, &allocation);
    rect.x = 0.0;
    rect.y = 0.0;
    rect.width = allocation.width;
    rect.height = allocation.height;

    slope_figure_draw(priv->figure, &rect, cr);
    return TRUE;
}


static
gboolean _view_on_button_press (GtkWidget *self, GdkEvent *event, gpointer data)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
    SlopeMouseEvent mouse_event;
    SLOPE_UNUSED(data);

    /* send event notification down to the figure's items */
    if (priv->figure != NULL) {
        mouse_event.x = event->button.x;
        mouse_event.y = event->button.y;
        if (event->button.button == 1) {
            mouse_event.type = SLOPE_MOUSE_LEFT_CLICK;
            _figure_mouse_event(priv->figure, &mouse_event);
        } else if (event->button.button == 3) {
            mouse_event.type = SLOPE_MOUSE_RIGHT_CLICK;
            _figure_mouse_event(priv->figure, &mouse_event);
        }
    }

    return TRUE;
}

/* slope/view.c */
