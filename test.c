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

#include <slope/slope.h>


int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *view;
    SlopeFigure *figure;
    SlopeScale *scale;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    view = slope_view_new();
    figure = slope_figure_new();
    scale = slope_scale_new();

    gtk_container_add(GTK_CONTAINER(window), view);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 450);

    slope_view_set_figure(SLOPE_VIEW(view), figure);
    slope_figure_add_scale(figure, scale);

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    slope_view_write_to_png(SLOPE_VIEW(view), "figure.png", 500, 450);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
