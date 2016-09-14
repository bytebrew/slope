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

#include <slope/chart.h>


int main(int argc, char *argv[])
{
    GtkWidget *chart;
    SlopeFigure *figure;
    SlopeScale *scale;

    gtk_init(&argc, &argv);
    chart = slope_chart_new();
    figure = slope_figure_new();

    g_signal_connect(G_OBJECT(chart), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(chart);
    gtk_main();

    return 0;
}
