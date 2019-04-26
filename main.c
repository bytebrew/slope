/*
 * Copyright (C) 2019  Elvis Teixeira
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

#include <gtk/gtk.h>
#include <slope/slope.h>

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *view;
    SlopeFigure *figure;

    window = gtk_application_window_new (app);
    view = slope_view_new();
    figure = slope_figure_new();
    slope_view_set_figure (SLOPE_VIEW (view), figure, TRUE);
    gtk_window_set_title (GTK_WINDOW (window), "Slope");
    gtk_window_set_default_size (GTK_WINDOW (window), 500, 400);
    gtk_container_add(GTK_CONTAINER (window), view);
    gtk_widget_show_all (window);
}


int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.slope.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

  return status;
}
