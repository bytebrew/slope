#include <slope/widget.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define NPTS 5000


int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *slope;

    slope_figure_t *figure;
    slope_scale_t *scale;
    slope_item_t *series;

    double x[NPTS], y[NPTS];
    int k;

    for (k=0; k<NPTS; k++) {
        x[k] = 10.0 * ((double) rand())/RAND_MAX;
        y[k] = 10.0 * ((double) rand())/RAND_MAX;
    }

    /* create series (plot) representation and place it in a scale */
    series = slope_series_new_for_data(x, y, NPTS, "Random scatter",
                  SLOPE_BLACK, SLOPE_GREEN, SLOPE_SERIES_CIRCLES);

    /* create a linear (cartesian) scale and set it's axis labels */
    scale = slope_linear_new("linear scale");
    slope_linear_set_axis_labels(scale, "X", "Crazy Data", "","");

    /* add the plots to the scale */
    slope_scale_add_item(scale, series);

    /* set up window */
    gtk_init (&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    slope = slope_widget_new();
    figure = slope_widget_get_figure(slope);
    /* get figure from widget and set it's scale and title */
    slope_figure_set_name(figure, "Some Data Points");
    slope_figure_add_scale(figure, scale);

    /* set window properties */
    gtk_container_add(GTK_CONTAINER(window), slope);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 350);
    gtk_widget_set_size_request(window, 200, 200);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* show the window */
    gtk_widget_show_all(window);
    gtk_main();

    /* clean up */
    slope_scale_destroy(scale);
    slope_item_destroy(series);
    return 0;
}

