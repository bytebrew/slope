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
#include <slope/grid.h>
#include <slope/axis2d.h>
#include <slope/series2d.h>
#include <slope/arrays.h>
#include <math.h>

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *view;
    SlopeFigure *figure;
    SlopeItem *grid;
    SlopeItem *axis1, *axis2, *axis3, *axis4, *axis5;
    SlopeItem *series1, *series2, *series3, *series4, *series5;
    SlopeArray2D *data1, *data2, *data3, *data4, *data5;
    double x;
    long k;

    gtk_init(&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    /* The figure view widget */
    view = slope_view_new();

    /* The figure to be shown */
    figure = slope_figure_new();

    /* Add a grid layout onto the figure */
    grid = slope_grid_new();
    slope_figure_set_root_item (figure, grid);

    /* Add axis onto the grid */
    axis1 = slope_axis2d_new("COS(X)");
    slope_grid_emplace (SLOPE_GRID (grid), axis1, 0, 0, 10, 10);

    /* Add another axis onto the grid */
    axis2 = slope_axis2d_new("SIN(X)");
    slope_grid_emplace (SLOPE_GRID (grid), axis2, 10, 0, 10, 10);

    /* Add another axis onto the grid */
    axis3 = slope_axis2d_new("LOG(1+X)");
    slope_grid_emplace (SLOPE_GRID (grid), axis3, 0, 10, 10, 10);

    /* Add another axis onto the grid */
    axis4 = slope_axis2d_new("EXP(X)");
    slope_grid_emplace (SLOPE_GRID (grid), axis4, 7, 7, 6, 6);

    axis5 = slope_axis2d_new("SIN(X) x COS(3X)");
    slope_grid_emplace (SLOPE_GRID (grid), axis5, 10, 10, 10, 10);

    /* create some datasets to plot */
    data1 = slope_array2d_new ();
    data2 = slope_array2d_new ();
    data3 = slope_array2d_new ();
    data4 = slope_array2d_new ();
    data5 = slope_array2d_new ();

    for (k = 0; k < 100; ++k) {
        x = k * 0.1;
        slope_array2d_append (data1, x, sin (x));
        slope_array2d_append (data2, x, cos (x));
        slope_array2d_append (data3, x, log (1.0 + x));
        slope_array2d_append (data4, x, exp (x));
        slope_array2d_append (data5, x, sin (x) * cos (3.0 * x));
    }

    /* Add data axis and put those axis onto the grid */
    series1 = slope_series2d_new_formatted ("b-");
    slope_series2d_set_data (SLOPE_SERIES2D (series1), data1, TRUE);
    slope_axis2d_add_plot (SLOPE_AXIS2D (axis1), SLOPE_PLOT2D (series1));

    series2 = slope_series2d_new_formatted ("r-");
    slope_series2d_set_data (SLOPE_SERIES2D (series2), data2, TRUE);
    slope_axis2d_add_plot (SLOPE_AXIS2D (axis2), SLOPE_PLOT2D (series2));

    series3 = slope_series2d_new_formatted ("k-");
    slope_series2d_set_data (SLOPE_SERIES2D (series3), data3, TRUE);
    slope_axis2d_add_plot (SLOPE_AXIS2D (axis3), SLOPE_PLOT2D (series3));

    series4 = slope_series2d_new_formatted ("g-");
    slope_series2d_set_data (SLOPE_SERIES2D (series4), data4, TRUE);
    slope_axis2d_add_plot (SLOPE_AXIS2D (axis4), SLOPE_PLOT2D (series4));

    series5 = slope_series2d_new_formatted ("b-");
    slope_series2d_set_data (SLOPE_SERIES2D (series5), data5, TRUE);
    slope_axis2d_add_plot (SLOPE_AXIS2D (axis5), SLOPE_PLOT2D (series5));

    /* Add the figureonto the view */
    slope_view_set_figure (SLOPE_VIEW (view), figure);

    /* Save to image files */
    slope_figure_save (figure, "Figure.png", 500, 400, "png");
    slope_figure_save (figure, "Figure.svg", 500, 400, "svg");

    /* Show on the window */
    gtk_container_add (GTK_CONTAINER (window), view);
    gtk_widget_show_all (window);
    gtk_main();

    return 0;
}
