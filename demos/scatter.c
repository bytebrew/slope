#include <slope/chart.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define NPTS 5000


int main(int argc, char *argv[])
{
    slope_chart_t *chart;

    double *x = malloc(NPTS*sizeof(double));
    double *y = malloc(NPTS*sizeof(double));
    int k;

    for (k=0; k<NPTS; k++) {
        x[k] = 10.0 * ((double) rand())/RAND_MAX;
        y[k] = 10.0 * ((double) rand())/RAND_MAX;
    }
    
    gtk_init (&argc, &argv);
    chart = slope_chart_new("Random scatter");

    /* create series (plot) representation and place it in a scale */
    slope_chart_plot (chart, x, y, NPTS, "Random scatter", "kor");
    slope_chart_show(chart);
    gtk_main();

    /* clean up */
    free(x);
    free(y);
    return 0;
}
