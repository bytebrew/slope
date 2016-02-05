/* slope includes */
#include <slope/widget.h>

/* gsl includes */
#include <gsl/gsl_fit.h>

/* standard C includes */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define NPTS 50


int main(int argc, char *argv[])
{
   GtkWidget *window;
   GtkWidget *slope;
   
   slope_figure_t *figure;
   slope_scale_t *scale;
   slope_item_t *legend;
   slope_item_t *data_series;
   slope_item_t *regression_series;
   double a, b, cov_00, cov_01, cov_11, chisqr;
   double *x, *y, *y_fit;
   int k;
   
   /* create synthetic data */
   x = malloc(NPTS*sizeof(double));
   y = malloc(NPTS*sizeof(double));
   for (k=0; k<NPTS; k++) {
      x[k] = 3*k;
      y[k] = 0.5*x[k] - 30.0 + 20.0*((double) rand())/RAND_MAX;
   }
   
   /* create series (plot representation) for data */
   data_series = slope_series_new_for_data(
      x, y, NPTS, "Experiment data",
      SLOPE_BLACK, SLOPE_BLUE, SLOPE_SERIES_CIRCLES);
   
   /* execute linear regression with gsl */
   gsl_fit_linear(x, 1, y, 1, NPTS, &a, &b, &cov_00, &cov_01, &cov_11, &chisqr);
   y_fit = malloc(NPTS*sizeof(double));
   for (k=0; k<NPTS; k++) {
      y_fit[k] = a + b*x[k];
   }
   
   /* create series for regression line */
   regression_series = slope_series_new_for_data(
      x, y_fit, NPTS, "Regression",
      SLOPE_RED, SLOPE_RED, SLOPE_SERIES_LINE);
   
   /* create a linear (cartesian) scale and set it's axis labels */
   scale = slope_linear_new("scale");
   slope_linear_set_axis_labels(scale, "X", "Y", "","");
   
   /* add the plots to the scale */
   slope_scale_add_item(scale, data_series);
   slope_scale_add_item(scale, regression_series);
   
   /* set up window */
   gtk_init (&argc, &argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   slope = slope_widget_new();
   figure = slope_widget_get_figure(slope);
   /* get figure from widget and set it's scale and title */
   slope_figure_set_name(figure, "Linear Regression");
   slope_figure_add_scale(figure, scale);
   /* put legend in the bottom */
   legend = slope_figure_get_legend(figure);
   slope_legend_set_position_policy(legend, SLOPE_LEGEND_BOTTOMRIGHT);
   
   /* set window properties */
   gtk_container_add(GTK_CONTAINER(window), slope);
   gtk_window_set_default_size(GTK_WINDOW(window), 500, 350);
   gtk_widget_set_size_request(window, 200, 200);
   g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
   
   /* show the window */
   gtk_widget_show_all(window);
   gtk_main();
   
   /* clean up */
   free(x);
   free(y);
   free(y_fit);
   slope_scale_destroy(scale);
   slope_item_destroy(data_series);
   slope_item_destroy(regression_series);
   return 0;
}
