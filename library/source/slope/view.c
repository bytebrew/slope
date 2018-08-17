/*
 * Copyright (C) 2018  Elvis Teixeira
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

#include "slope/view.h"

typedef struct _SlopeViewPrivate SlopeViewPrivate;
#define SLOPE_VIEW_PRIVATE(Addr) ((SlopeViewPrivate*) (Addr))

struct _SlopeViewPrivate
{
    SlopeFigure *figure;
};

G_DEFINE_TYPE_WITH_PRIVATE (SlopeView, slope_view, GTK_TYPE_DRAWING_AREA)
#define SLOPE_VIEW_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_TYPE_VIEW, SlopeViewPrivate))

/* local decls */
static void slope_view_finalize(GObject *self);
static void slope_view_dispose (GObject *self);
static gboolean slope_view_draw(GtkWidget *self, cairo_t *cr, gpointer data);
static void on_figure_change (GObject *figure, gpointer user_data);

static void
slope_view_class_init (SlopeViewClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose = slope_view_dispose;
    gobject_class->finalize = slope_view_finalize;
}

static void
slope_view_init (SlopeView *view)
{
    GtkWidget * gtk_widget = GTK_WIDGET(view);
    SlopeViewPrivate *m = SLOPE_VIEW_GET_PRIVATE (view);

    /* expect figure from user */
    m->figure = NULL;

    /* Minimum width and height to be meaningful */
    gtk_widget_set_size_request(gtk_widget, 450, 380);

    /* select the types of events we want to be notified about */
    gtk_widget_add_events(gtk_widget,
                          GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK |
                          GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);

    g_signal_connect (view, "draw", G_CALLBACK (slope_view_draw), view);
}


static void
slope_view_dispose (GObject *object)
{
  SlopeView *self = SLOPE_VIEW (object);
  SlopeViewPrivate *m = SLOPE_VIEW_GET_PRIVATE (self);

  if (m->figure != NULL) {
      g_object_unref(G_OBJECT (m->figure));
      m->figure = NULL;
  }

  G_OBJECT_CLASS (slope_view_parent_class)->dispose (object);
}


static void
slope_view_finalize(GObject *object)
{
    SlopeView *self = SLOPE_VIEW (object);
    SlopeViewPrivate *m = SLOPE_VIEW_GET_PRIVATE (object);

    // TODO

    G_OBJECT_CLASS (slope_view_parent_class)->finalize (object);
}


GtkWidget*
slope_view_new (void)
{
 return g_object_new (SLOPE_TYPE_VIEW, NULL);
}


static gboolean slope_view_draw(GtkWidget *self, cairo_t *cr, gpointer data)
{
    SlopeViewPrivate *m = SLOPE_VIEW_GET_PRIVATE (self);
    GtkAllocation alloc;
    SLOPE_UNUSED(data);

    if (m->figure == NULL) {
        return TRUE;
    }

    gtk_widget_get_allocation(self, &alloc);
    slope_figure_draw(m->figure, cr, alloc.width, alloc.height);

    return TRUE;
}


void
slope_view_set_figure (SlopeView *self, SlopeFigure *figure)
{
    g_return_if_fail(SLOPE_IS_VIEW(self));
    SLOPE_VIEW_GET_PRIVATE (self)->figure = figure;
    g_signal_connect (G_OBJECT (figure), "changed", (GCallback) on_figure_change, self);
}


SlopeFigure*
slope_view_get_figure (SlopeView *self)
{
    g_return_val_if_fail(SLOPE_IS_VIEW(self), NULL);
    return SLOPE_VIEW_GET_PRIVATE (self)->figure;
}


static void
on_figure_change (GObject *figure, gpointer user_data)
{
    GtkWidget *view = GTK_WIDGET (user_data);
    SLOPE_UNUSED (figure);
    gtk_widget_queue_draw (view);
}

/* slope/view.c */
