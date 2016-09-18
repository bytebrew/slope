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


static SlopeXyAxisSample
x_samples[] = {
    { 0.0*G_PI, "0"},
    { 0.5*G_PI, "π/2"},
    { 1.0*G_PI, "π"},
    { 1.5*G_PI, "3π/2"},
    { 2.0*G_PI, "2π"},
    { 2.5*G_PI, "5π/2"},
    { 3.0*G_PI, "3π"},
    { 3.5*G_PI, "7π/2"},
    { 4.0*G_PI, "4π"}
};

static SlopeXyAxisSample
y_samples[] = {
    { -1.0, "-1"},
    { -0.5, "-0.5"},
    { 0.0, "0"},
    { 0.5, "0.5"},
    { 1.0, "1"}
};


int main(int argc, char *argv[])
{
    GtkWidget *chart;
    SlopeFigure *figure;
    SlopeScale *scale1, *scale2, *scale3;
    SlopeItem *series1, *series2, *series3;
    SlopeItem *axis;
    SlopeXyAxisSampler *sampler;
    double *x, *y1, *y2, *y3;

    gtk_init(&argc, &argv);
    chart = slope_chart_new();

    g_signal_connect(G_OBJECT(chart), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    /* create some sinusoidal data points */
    long k, n = 50;
    x = g_malloc(n * sizeof(double));
    y1 = g_malloc(n * sizeof(double));
    y2 = g_malloc(n * sizeof(double));
    y3 = g_malloc(n * sizeof(double));
    double dx = 4.0 * G_PI / n;

    for (k=0; k<n; ++k) {
        x[k] = k * dx;
        y1[k] = sin(x[k]);
        y2[k] = 1.0 + y1[k] + 0.1 * k;
        y3[k] = g_random_int();
    }

    figure = slope_chart_get_figure(SLOPE_CHART(chart));

    scale1 = slope_xyscale_new();
    slope_scale_set_name(scale1, "Sinusoidal");
    slope_scale_set_layout_rect(scale1, 0, 0, 1, 1);
    slope_figure_add_scale(figure, scale1);
    axis = slope_xyscale_get_axis(SLOPE_XYSCALE(scale1), SLOPE_XYSCALE_AXIS_BOTTOM);
    sampler = slope_xyaxis_get_sampler(SLOPE_XYAXIS(axis));
    slope_xyaxis_sampler_set_samples(sampler, x_samples, 9);
    axis = slope_xyscale_get_axis(SLOPE_XYSCALE(scale1), SLOPE_XYSCALE_AXIS_LEFT);
    sampler = slope_xyaxis_get_sampler(SLOPE_XYAXIS(axis));
    slope_xyaxis_sampler_set_samples(sampler, y_samples, 5);

    scale2 = slope_xyscale_new();
    slope_scale_set_name(scale2, "Sine + Linear");
    slope_scale_set_layout_rect(scale2, 0, 1, 1, 1);
    slope_figure_add_scale(figure, scale2);
    slope_xyscale_set_visible_axis(SLOPE_XYSCALE(scale2), SLOPE_XYSCALE_NO_AXIS);

    scale3 = slope_xyscale_new();
    slope_scale_set_name(scale3, "Random Scatter");
    slope_scale_set_layout_rect(scale3, 0, 2, 1, 1);
    slope_figure_add_scale(figure, scale3);
    slope_xyscale_set_visible_axis(SLOPE_XYSCALE(scale3), SLOPE_XYSCALE_NO_AXIS);

    series1 = slope_xyseries_new_filled("Sine", x, y1, n, "b-");
    slope_scale_add_item(scale1, series1);

    series2 = slope_xyseries_new_filled("Sine + Linear", x, y2, n, "ga");
    slope_scale_add_item(scale2, series2);

    series3 = slope_xyseries_new_filled("Scatter", x, y3, n, "ro");
    slope_scale_add_item(scale3, series3);

    slope_figure_write_to_png(figure, "figure.png", 500, 450);
    gtk_widget_show_all(chart);
    gtk_main();

    g_free(x);
    g_free(y1);
    g_free(y2);
    g_free(y3);

    return 0;
}
