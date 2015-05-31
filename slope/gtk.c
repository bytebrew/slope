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
    GtkWidget *view = slope_view_new_for_figure(figure);
    gtk_container_add(GTK_CONTAINER(window), view);
    g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    return window;
}

/* slope/gtk.h */
