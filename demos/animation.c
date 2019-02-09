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

#include <slope/view.h>
#include <slope/grid.h>
#include <slope/axis2d.h>
#include <slope/series2d.h>
#include <slope/arrays.h>
#include <math.h>

static SlopePoint *pts;
static const long   n  = 200;
static const double dx = 4.0 * G_PI / 200;
static GtkWidget *view;
static SlopeItem *series;
static SlopeItem *axis;

static gboolean
timer_callback(GtkWidget *view)
{
  static long count = 0;
  count++;

  long k;
  for (k = 0; k < n; ++k)
    {
      pts[k].y = sin(pts[k].x + 0.1 * count) + sin(1.2 * pts[k].x - 0.1 * count);
    }

  slope_axis2d_update_scale (SLOPE_AXIS2D (axis));
  slope_view_redraw (SLOPE_VIEW (view));
  return TRUE;
}


int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *view;
    SlopeFigure *figure;
    SlopeArray2D *data;
    double x;
    long k;

    gtk_init(&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(window), 560, 380);

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    /* The figure view widget */
    view = slope_view_new();

    /* The figure to be shown */
    figure = slope_figure_new();

    /* Add axis onto the grid */
    axis = slope_axis2d_new("Waves");
    slope_figure_set_root_item (figure, axis);

    /* create some datasets to plot */
    data = slope_array2d_new ();

    for (k = 0; k < n; ++k) {
        x = -10.0 + k * 0.1;
        slope_array2d_append (data, x, sin(x) + sin(1.2 * x));
    }

    gulong npts;
    slope_array2d_get_points (data, &npts, &pts);

    /* Add data axis and put those axis onto the grid */
    series = slope_series2d_new_formatted ("b-");
    slope_series2d_set_data (SLOPE_SERIES2D (series), data, TRUE);
    slope_axis2d_add_plot (SLOPE_AXIS2D (axis), SLOPE_PLOT2D (series));

    /* Add the figureonto the view */
    slope_view_set_figure (SLOPE_VIEW (view), figure);

    /* Save to image files */
    slope_figure_save (figure, "Animation.png", 500, 400, "png");
    slope_figure_save (figure, "Animation.svg", 500, 400, "svg");

    /* Show on the window */
    gtk_container_add (GTK_CONTAINER (window), view);
    gtk_widget_show_all (window);

    g_timeout_add(1000/25, (GSourceFunc) timer_callback, (gpointer) view);
    gtk_main();

    return 0;
}
