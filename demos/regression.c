/* slope includes */
#include <slope/chart.h>

/* gsl includes */
#include <gsl/gsl_fit.h>

/* standard C includes */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define NPTS 50


int main(int argc, char *argv[])
{
   slope_chart_t *chart;
   double a, b, cov_00, cov_01, cov_11, chisqr;
   double *x, *y, *y_fit;
   int k;

   gtk_init (&argc, &argv);
   chart = slope_chart_new("Linear Regression");

   /* create synthetic data */
   x = malloc(NPTS*sizeof(double));
   y = malloc(NPTS*sizeof(double));
   for (k=0; k<NPTS; k++) {
      x[k] = 3*k;
      y[k] = 0.5*x[k] - 30.0 + 20.0*((double) rand())/RAND_MAX;
   }

   /* create series (plot representation) for data */
   slope_chart_plot (chart, x, y, NPTS, "Experiment data", "ro");
   
   /* execute linear regression with gsl */
   gsl_fit_linear(x, 1, y, 1, NPTS, &a, &b, &cov_00, &cov_01, &cov_11, &chisqr);
   y_fit = malloc(NPTS*sizeof(double));
   for (k=0; k<NPTS; k++) {
      y_fit[k] = a + b*x[k];
   }

   /* create series for regression line */
   slope_chart_plot (chart, x, y_fit, NPTS, "Regression","b-");
   
   slope_chart_show(chart);
   gtk_main();

   /* clean up */
   free(x);
   free(y);
   free(y_fit);
   return 0;
}
