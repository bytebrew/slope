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

#include "slope/slope.h"
#include <stdlib.h>


slope_figure_t*
slope_chart_create (const char *title,
                    const char *xlabel,
                    const char *ylabel)
{
  slope_figure_t *figure = slope_figure_create();
  slope_metrics_t *metrics = slope_xymetrics_create();
  slope_item_set_name(
      slope_xymetrics_get_axis(metrics, SLOPE_XYAXIS_TOP), title);
  slope_item_set_name(
      slope_xymetrics_get_axis(metrics, SLOPE_XYAXIS_BOTTOM), xlabel);
  slope_item_set_name(
      slope_xymetrics_get_axis(metrics, SLOPE_XYAXIS_LEFT), ylabel);
  slope_figure_add_metrics(figure, metrics);
  return figure;
}


void
slope_chart_destroy (slope_figure_t *figure)
{
  if (figure == NULL) {
    return;
  }
  slope_iterator_t *met_iter =
    slope_list_first(slope_figure_get_metrics_list(figure));
  while (met_iter) {
    slope_metrics_t *metrics =
      (slope_metrics_t*) slope_iterator_data(met_iter);
    slope_iterator_t *plot_iter =
      slope_list_first(slope_metrics_get_item_list(metrics));
    while (plot_iter) {
      slope_item_t *plot =
        (slope_item_t*) slope_iterator_data(plot_iter);
      slope_item_destroy(plot);
      slope_iterator_next(&plot_iter);
    }
    slope_metrics_destroy(metrics);
    slope_iterator_next(&met_iter);
  }
  slope_figure_destroy(figure);
}


slope_item_t*
slope_chart_add_plot (slope_figure_t *chart,
                      const double *x, const double *y, int n,
                      const char *title, const char *fmt)
{
  slope_item_t *plot =
    slope_funcplot_create_simple(x, y, n, title, fmt);
  slope_iterator_t *iter =
    slope_list_first(slope_figure_get_metrics_list(chart));
  slope_metrics_t *metrics =
    (slope_metrics_t*) slope_iterator_data(iter);
  slope_metrics_add_item(metrics, plot);
  return plot;
}


#if SLOPE_HAVE_GTK
GtkWidget*
slope_create_window (slope_figure_t *figure,
                     const char *title)
{
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 350);
  gtk_window_set_title(GTK_WINDOW(window), title);
  gtk_widget_set_size_request(window, 200, 200);
  GtkWidget *view = slope_view_new_for_figure(figure, SLOPE_TRUE);
  gtk_container_add(GTK_CONTAINER(window), view);
  g_signal_connect(G_OBJECT(window), "delete-event",
    G_CALLBACK(gtk_main_quit), NULL);
  return window;
}
#endif /* SLOPE_HAVE_GTK */

/* slope/slope.h */
