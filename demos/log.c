#include <math.h>
#include <gtk/gtk.h>
#include <slope/widget.h>
#include <stdio.h>


#define NPTS 50


int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *slope;

    slope_figure_t *figure;
    slope_scale_t *scale;
    slope_item_t *legend;
    slope_item_t *log1_series;
    slope_item_t *log2_series;
    slope_item_t *log3_series;
    
    const double step = 2.0*M_PI/(NPTS-1);
    double x[NPTS], y1[NPTS], y2[NPTS], y3[NPTS];
    int k;
    for (k=0; k<NPTS; k++) {
        x[k] = 1.0 + k*step;
        y1[k] = log(x[k]);
        y2[k] = log(2.0*x[k]);
        y3[k] = log(3.0*x[k]);
    }

    /* create series (plot) representation and place it in a scale */
    log1_series = slope_series_new_for_data(x, y1, NPTS, "log(x)", SLOPE_BLACK, SLOPE_RED, SLOPE_SERIES_CIRCLES);
    log2_series = slope_series_new_for_data(x, y2, NPTS, "log(2x)", SLOPE_BLACK, SLOPE_YELLOW, SLOPE_SERIES_CIRCLES);
    log3_series = slope_series_new_for_data(x, y3, NPTS, "log(3x)", SLOPE_BLACK, SLOPE_BLUE, SLOPE_SERIES_CIRCLES);

    /* create a linear (cartesian) scale and set it's axis labels */
    scale = slope_linear_new("linear scale");
    slope_linear_set_axis_labels(scale, "X", "Log(NX)", "","");
    
    /* add the plots to the scale */
    slope_scale_add_item(scale, log1_series);
    slope_scale_add_item(scale, log2_series);
    slope_scale_add_item(scale, log3_series);

    /* set up window */
    gtk_init (&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    slope = slope_widget_new();
    figure = slope_widget_get_figure(slope);
    /* get figure from widget and set it's scale and title */
    slope_figure_set_name(figure, "Some logarithm functions");
    slope_figure_add_scale(figure, scale);
    slope_figure_set_color_scheme(figure, SLOPE_BLACK, SLOPE_WHITE, SLOPE_WHITE);
    /* put legend bellow the curves */
    legend = slope_figure_get_legend(figure);
    slope_legend_set_position_policy(legend, SLOPE_LEGEND_BOTTOMRIGHT);

    gtk_container_add(GTK_CONTAINER(window), slope);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 350);
    gtk_widget_set_size_request(window, 200, 200);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    /* clean up */
    slope_scale_destroy(scale);
    slope_item_destroy(log1_series);
    slope_item_destroy(log2_series);
    slope_item_destroy(log3_series);
    return 0;
}
