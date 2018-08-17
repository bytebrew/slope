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

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *view;
    SlopeFigure *figure;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    view = slope_view_new();
    figure = slope_figure_new();

    /* use of GObject properties */
    GValue stroke_color = G_VALUE_INIT;
    g_value_init(&stroke_color, G_TYPE_UINT);
    g_value_set_uint(&stroke_color, SLOPE_TEAL);
    g_object_set_property(G_OBJECT (figure), "bg-stroke-color", &stroke_color);

    slope_view_set_figure (SLOPE_VIEW (view), figure);
    slope_figure_save (figure, "Figure.png", 500, 400, "png");
    gtk_window_set_title (GTK_WINDOW (window), slope_figure_get_title (SLOPE_FIGURE (figure)));

    gtk_container_add(GTK_CONTAINER(window), view);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
