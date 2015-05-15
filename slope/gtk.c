/*
 * Copyright (C) 2015  Elvis Teixeira
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
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "slope/gtk.h"


static gboolean on_draw (GtkWidget *widget, cairo_t *cr, gpointer *item);


GtkWidget* slope_create_window (slope_figure_t *figure,
                                const char *title)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 350);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_widget_set_size_request(window, 200, 200);
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), figure);

    return window;
}


static gboolean on_draw (GtkWidget *widget, cairo_t *cr, gpointer *item)
{
    slope_rect_t rect;
    rect.x = 0.0;
    rect.y = 0.0;
    rect.width = (double) gtk_widget_get_allocated_width(widget);
    rect.height = (double) gtk_widget_get_allocated_height(widget);
    slope_figure_t *figure = (slope_figure_t*) item;
    slope_figure_draw(figure, cr, &rect);
    return TRUE;
}

/* slope/gtk.h */
