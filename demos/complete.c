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
#include <math.h>


int main(int argc, char *argv[])
{
    GtkWidget *chart;
    SlopeFigure *figure;
    SlopeScale *scale;
    SlopeItem *series1, *series2;
    double *x, *y1, *y2;

    gtk_init(&argc, &argv);
    chart = slope_chart_new();

    g_signal_connect(G_OBJECT(chart), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    figure = slope_chart_get_figure(SLOPE_CHART(chart));
    scale = slope_xyscale_new();

    slope_scale_set_name(scale, "Data Series 43");
    slope_figure_add_scale(figure, scale);

    /* create some sinusoidal data points */
    long k, n = 40;
    x = g_malloc(n * sizeof(double));
    y1 = g_malloc(n * sizeof(double));
    y2 = g_malloc(n * sizeof(double));
    double dx = 2.0 * G_PI / n;

    for (k=0; k<n; ++k) {
        x[k] = k * dx;
        y1[k] = k + 10 * sin(3.0 * x[k]);
        y2[k] = 0.05 * k*k;
    }

    series1 = slope_xyseries_new_filled("Sinu-linear", x, y1, n, "0ag");
    series2 = slope_xyseries_new_filled("Quadratic", x, y2, n, "kor");
    slope_scale_add_item(scale, series1);
    slope_scale_add_item(scale, series2);

    slope_figure_write_to_png(figure, "figure.png", 500, 450);
    gtk_widget_show_all(chart);
    gtk_main();

    return 0;
}
