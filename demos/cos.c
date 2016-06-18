/* This example shws how to use custom axis tick labels
   in this case the multiples of pi */

#include <math.h>
#include <gtk/gtk.h>
#include <slope/chart.h>
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
    slope_chart_t *chart;
    slope_item_t *axis;
    slope_sampler_t *sampler;
    
    gtk_init (&argc, &argv);
    chart = slope_chart_new ("Cossine curve");
    
    const double step = 2.0*M_PI/(NPTS-1);
    double x[NPTS], y[NPTS];
    int k;
    for (k=0; k<NPTS; k++) {
        x[k] = k*step;
        y[k] = cos(x[k]);
    }

    /* create series (plot) representation and place it in a scale */
    slope_chart_plot (chart, x, y, NPTS, "cos(x)", "bag");

    /* set the bottom axis sample labels as unicode symbols */
    axis = slope_chart_get_axis(chart, SLOPE_AXIS_BOTTOM);
    sampler = slope_axis_get_sampler(axis);
    slope_sampler_set_samples(sampler, x_samples, 5);

    slope_chart_show(chart);
    gtk_main();
    return 0;
}

