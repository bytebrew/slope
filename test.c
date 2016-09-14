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
    GtkWidget *chart;
    SlopeFigure *figure;
    SlopeScale *scale1, *scale2;

    gtk_init(&argc, &argv);
    chart = slope_chart_new();

    scale1 = slope_xyscale_new();
    scale2 = slope_xyscale_new();

    g_signal_connect(G_OBJECT(chart), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    figure = slope_chart_get_figure(SLOPE_CHART(chart));

    slope_scale_set_layout_rect(scale1, 0, 0, 1, 1);
    slope_scale_set_background_color(scale1, SLOPE_LIGHTSTEELBLUE);
    slope_figure_add_scale(figure, scale1);

    slope_scale_set_layout_rect(scale2, 0, 1, 1, 1);
    slope_scale_set_background_color(scale2, SLOPE_PALETURQUOISE);
    slope_figure_add_scale(figure, scale2);

    slope_figure_write_to_png(figure, "figure.png", 500, 450);
    gtk_widget_show_all(chart);
    gtk_main();

    return 0;
}
