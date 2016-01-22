#include <math.h>
#include <gtk/gtk.h>
#include <slope/widget.h>
#include <stdio.h>


#define NPTS 50


static const slope_sample_t x_samples[] = {
    {0.0,          "0"},
    {M_PI/2.0,     "π/2"},
    {M_PI,         "π"},
    {3.0*M_PI/2.0, "3π/2"},
    {2.0*M_PI,     "2π"}
};


int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *slope;

    slope_figure_t *figure;
    slope_scale_t *scale;
    slope_item_t *cos_series;
    slope_item_t *axis;
    slope_sampler_t *sampler;
    
    const double step = 2.0*M_PI/(NPTS-1);
    double x[NPTS], y[NPTS];
    int k;
    for (k=0; k<NPTS; k++) {
        x[k] = k*step;
        y[k] = cos(x[k]);
    }

    /* create series (plot) representation and place it in a scale */
    cos_series = slope_series_new_for_data(x, y, NPTS, "cos(x)", SLOPE_BLUE, SLOPE_ORANGE, SLOPE_SERIES_AREAUNDER);
    
    /* create a linear (cartesian) scale and set it's axis labels */
    scale = slope_linear_new("linear scale");
    slope_linear_set_axis_labels(scale, "X (RADIANS)", "Cos(X)", "","");
    slope_linear_set_show_grid(scale, SLOPE_TRUE);
    slope_scale_add_item(scale, cos_series);

    /* set axis custom sample ticks */
    axis = slope_linear_get_axis(scale, SLOPE_AXIS_BOTTOM);
    sampler = slope_axis_get_sampler(axis);
    slope_sampler_set_samples(sampler, x_samples, 5);

    /* set up window */
    gtk_init (&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    slope = slope_widget_new();
    figure = slope_widget_get_figure(slope);
    slope_figure_set_name(figure, "Area under a cossine curve");
    slope_figure_add_scale(figure, scale);

    gtk_container_add(GTK_CONTAINER(window), slope);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 350);
    gtk_widget_set_size_request(window, 200, 200);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    /* clean up */
    slope_scale_destroy(scale);
    slope_item_destroy(cos_series);
    return 0;
}
