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
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <slope/chart.h>
#include <stdlib.h>


struct _slope_chart {
    GtkWidget *window;
    GtkWidget *slope;
    GtkWidget *header;
    slope_scale_t *scale;
    slope_figure_t *figure;
};


static slope_list_t *window_list = NULL;
static gboolean on_close (GtkWidget *widget, gpointer data);


slope_chart_t* slope_chart_new (const char *title)
{
    GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *header = gtk_header_bar_new();
    GtkWidget *slope = slope_widget_new();

    slope_chart_t *chart = SLOPE_ALLOC(slope_chart_t);
    slope_scale_t *scale = slope_linear_new("linear scale");
    slope_figure_t *figure = slope_widget_get_figure(slope);

    slope_figure_add_scale(figure, scale);
    slope_figure_set_name(figure, title);

    gtk_header_bar_set_title(GTK_HEADER_BAR(header), title);
    gtk_header_bar_set_subtitle(GTK_HEADER_BAR(header), "Slope - Gtk");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
    gtk_window_set_titlebar(GTK_WINDOW(window), header);
    gtk_container_add (GTK_CONTAINER(window), slope);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);
    gtk_widget_set_size_request(window, 300, 250);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_close), NULL);

    chart->window = window;
    chart->header = header;
    chart->slope = slope;
    chart->figure = figure;
    chart->scale = scale;
    return chart;
}


void slope_chart_plot (slope_chart_t *chart, const double *vx, const double *vy,
                       int size, const char *name, const char *style)
{
    slope_item_t *series;

    if (!chart) return;
    series = slope_series_new (vx, vy, size, name, style);
    slope_scale_add_item(chart->scale, series);
}


void slope_chart_show (slope_chart_t *chart)
{
    if (!chart) return;
    if (!window_list)
        window_list = slope_list_new();
    slope_list_append(window_list, chart->window);
    gtk_widget_show_all(chart->window);
}


void slope_chart_destroy (slope_chart_t *chart)
{
    if (!chart) return;
    /* TODO */
}


static gboolean on_close (GtkWidget *widget, gpointer data)
{
    slope_list_remove(window_list, widget);
    if (slope_list_size(window_list) == 0)
        gtk_main_quit();
}


slope_figure_t* slope_chart_get_figure (slope_chart_t *self)
{
    if (self)
        return self->figure;
    return NULL;
}


slope_item_t* slope_chart_get_axis (slope_chart_t *self, slope_axis_position_t pos)
{
    if (self)
        return slope_linear_get_axis(self->scale, pos);
    return NULL;
}

/* slope/chart.c */
