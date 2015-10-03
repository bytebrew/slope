/*
 * Writen by Elvis M. Teixeira
 *
 * This program is in the public domain
 */

#include <slope/slope.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define N 100


int main (int argc, char *argv[])
{
    /* generate some item */
    const double step = 2.0*M_PI/N;
    double x[N], y1[N], y2[N];
    int k;
    
    for (k=0; k<N; k++) {
        x[k] = k*step;
        y1[k] = sin(x[k]);
        y2[k] = y1[k] + 0.2*((double)rand())/RAND_MAX - 0.1;
    }

    /* this is all you need to know to crete a chart */
    slope_figure_t *chart = slope_chart_create("Sine and Random Noise", "phase", "amplitude");
    slope_chart_add_plot(chart, x, y2, N, "Noisy experiment data", "l+");
    slope_chart_add_plot(chart, x, y1, N, "Sine approximation", "r-");

    #if SLOPE_HAVE_GTK
    /* put the scene in a gtk widget if you want */
    gtk_init(&argc,&argv);
    GtkWidget *window = slope_create_window(chart, "Data and Model");
    gtk_widget_show_all(window);
    gtk_main();
    #endif /* SLOPE_HAVE_GTK */
    
    /* and save the chart in some image format */
    slope_figure_write_to_png(chart, "figure.png", 500, 350);
    slope_figure_write_to_ps(chart, "figure.ps", 500, 350);
    slope_figure_write_to_pdf(chart, "figure.pdf", 500, 350);
    slope_figure_write_to_svg(chart, "figure.svg", 500, 350);

    slope_chart_destroy(chart);
    return 0;
}

