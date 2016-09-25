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
    gboolean mouse_pressed;
}
SlopeViewPrivate;


#define SLOPE_VIEW_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_VIEW_TYPE, SlopeViewPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeView, slope_view, GTK_TYPE_DRAWING_AREA)


static void _view_finalize (GObject *self);
static void _view_set_figure (SlopeView *self, SlopeFigure *figure);
static gboolean _view_on_draw (GtkWidget *self, cairo_t *cr, gpointer data);
static gboolean _view_on_mouse_press (GtkWidget *self, GdkEvent *event, gpointer data);
static gboolean _view_on_mouse_move (GtkWidget *self, GdkEvent *event, gpointer data);
static gboolean _view_on_mouse_release (GtkWidget *self, GdkEvent *event, gpointer data);



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
    priv->mouse_pressed = FALSE;

    gtk_widget_set_size_request(gtk_widget, 250, 250);
    gtk_widget_add_events(gtk_widget,
                GDK_EXPOSURE_MASK
                |GDK_BUTTON_PRESS_MASK
                |GDK_BUTTON_RELEASE_MASK
                |GDK_POINTER_MOTION_MASK);

    g_signal_connect(G_OBJECT(self), "draw",
                     G_CALLBACK(_view_on_draw), NULL);
    g_signal_connect(G_OBJECT(self), "button-press-event",
                     G_CALLBACK(_view_on_mouse_press), NULL);
    g_signal_connect(G_OBJECT(self), "motion-notify-event",
                     G_CALLBACK(_view_on_mouse_move), NULL);
    g_signal_connect(G_OBJECT(self), "button-release-event",
                     G_CALLBACK(_view_on_mouse_release), NULL);
}


static
void _view_finalize (GObject *self)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);

    if (priv->figure != NULL) {
        if (slope_figure_get_is_managed(priv->figure)) {
            g_object_unref(priv->figure);
        }
        priv->figure = NULL;
    }

    G_OBJECT_CLASS(slope_view_parent_class)->finalize(self);
}


GtkWidget* slope_view_new ()
{
    GtkWidget *self = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));

    return self;
}


GtkWidget* slope_view_new_with_figure (SlopeFigure *figure)
{
    GtkWidget *self = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));

    slope_view_set_figure(SLOPE_VIEW(self), figure);

    return self;
}


static
void _view_set_figure (SlopeView *self, SlopeFigure *figure)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);

    // TODO
    priv->figure = figure;
    _figure_set_view(figure, self);
}


void slope_view_redraw (SlopeView *self)
{
    gtk_widget_queue_draw(GTK_WIDGET(self));
}


SlopeFigure* slope_view_get_figure (SlopeFigure *self)
{
    if (self != NULL) {
        return SLOPE_VIEW_GET_PRIVATE(self)->figure;
    }
    return NULL;
}


void slope_view_write_to_png (SlopeView *self, const char *filename, int width, int height)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);

    if (priv->figure != NULL) {
        slope_figure_write_to_png(priv->figure, filename, width, height);
    }
}


static
gboolean _view_on_draw (GtkWidget *self, cairo_t *cr, gpointer data)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
    GtkAllocation allocation;
    SlopeRect rect;
    SLOPE_UNUSED(data);

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
gboolean _view_save_png_dialog (GtkWidget *self, gpointer data)
{
    SlopeView *view = SLOPE_VIEW(data);
    GtkWidget *parent_window = gtk_widget_get_toplevel(GTK_WIDGET(view));
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(data);
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    GtkWidget *save_dialog;
    GtkFileFilter *filter;
    gint res;
    SLOPE_UNUSED(self);

    filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.png");

    save_dialog = gtk_file_chooser_dialog_new(
          "Save PNG",
          GTK_WINDOW(parent_window),
          action,
          "_Cancel",
          GTK_RESPONSE_CANCEL,
          "_Open",
          GTK_RESPONSE_ACCEPT,
          NULL);

    gtk_file_chooser_set_filter(
        GTK_FILE_CHOOSER(save_dialog), filter);
    gtk_file_chooser_set_do_overwrite_confirmation(
        GTK_FILE_CHOOSER(save_dialog), TRUE);

    res = gtk_dialog_run(GTK_DIALOG(save_dialog));

    if (res == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(save_dialog);
        char *filename;
        GtkAllocation alloc;

        gtk_widget_get_allocation(GTK_WIDGET(view), &alloc);

        filename = gtk_file_chooser_get_filename(chooser);
        slope_figure_write_to_png(priv->figure, filename, alloc.width, alloc.height);
        g_free(filename);
    }

    gtk_widget_destroy(save_dialog);
    return TRUE;
}


static
gboolean _view_show_context_menu (GtkWidget *self, GdkEvent *event)
{
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *save_option = gtk_menu_item_new_with_label("Save PNG");

    g_signal_connect(G_OBJECT(save_option), "activate",
                     G_CALLBACK(_view_save_png_dialog), self);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), save_option);
    gtk_widget_show_all(menu);
    gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
                   event->button.button, event->button.time);

    return TRUE;
}


static
gboolean _view_on_mouse_press (GtkWidget *self, GdkEvent *event, gpointer data)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
    SlopeViewMouseEvent item_event;
    SLOPE_UNUSED(data);

    priv->mouse_pressed = TRUE;
    if (priv->figure == NULL) {
        return TRUE;
    }

    if (event->button.button == 1) {
        item_event.buttom = SLOPE_VIEW_LEFT_BUTTON;
    }
    else if (event->button.button == 3) {
        item_event.buttom = SLOPE_VIEW_RIGHT_BUTTON;

        if (event->type != GDK_2BUTTON_PRESS) {
            if (_view_show_context_menu(self, event) == TRUE) {
                return TRUE;
            }
        }
    }
    else {
        return TRUE;
    }

    item_event.type = event->type == GDK_2BUTTON_PRESS
               ? SLOPE_VIEW_BUTTON_DOUBLE_PRESS
               : SLOPE_VIEW_BUTTON_PRESS;

    item_event.x = event->button.x;
    item_event.y = event->button.y;

    _figure_handle_mouse_event(priv->figure, &item_event);
    return TRUE;
}


static
gboolean _view_on_mouse_move (GtkWidget *self, GdkEvent *event, gpointer data)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
    SlopeViewMouseEvent item_event;
    SLOPE_UNUSED(data);

    if (priv->figure == NULL) {
        return TRUE;
    }

    if (event->button.button == 1) {
        item_event.buttom = SLOPE_VIEW_LEFT_BUTTON;
    }
    else if (event->button.button == 3) {
        item_event.buttom = SLOPE_VIEW_RIGHT_BUTTON;
    }
    else {
        item_event.buttom = SLOPE_VIEW_NO_BUTTON;
    }

    item_event.type = (priv->mouse_pressed == TRUE)
            ? SLOPE_VIEW_MOVE_PRESSED
            : SLOPE_VIEW_MOVE;

    item_event.x = event->button.x;
    item_event.y = event->button.y;

    _figure_handle_mouse_event(priv->figure, &item_event);
    return TRUE;
}


static
gboolean _view_on_mouse_release (GtkWidget *self, GdkEvent *event, gpointer data)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_GET_PRIVATE(self);
    SlopeViewMouseEvent item_event;
    SLOPE_UNUSED(data);

    priv->mouse_pressed = FALSE;
    if (priv->figure == NULL) {
        return TRUE;
    }

    if (event->button.button == 1) {
        item_event.buttom = SLOPE_VIEW_LEFT_BUTTON;
    }
    else if (event->button.button == 3) {
        item_event.buttom = SLOPE_VIEW_RIGHT_BUTTON;
    }
    else {
        return TRUE;
    }

    item_event.type = SLOPE_VIEW_BUTTON_RELEASE;
    item_event.x = event->button.x;
    item_event.y = event->button.y;

    _figure_handle_mouse_event(priv->figure, &item_event);
    return TRUE;
}


void slope_view_set_figure (SlopeView *self, SlopeFigure *figure)
{
    SLOPE_VIEW_GET_CLASS(self)->set_figure(self, figure);
}

/* slope/view.c */
