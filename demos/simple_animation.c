/*
 *Copyright (C) 2017 Nuno Ferreira
 *Copyright (C) 2016 Elvis Teixeira
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

#include <slope/slope.h>
#include <math.h>


SlopeScale *scale;
SlopeItem *series;
double *x, *y;
double dx;
GtkWidget *chart;

/*Animated plot Y-axis SCALE*/
Max_scale = 2.5;        
    


gboolean timer = TRUE;

static void do_drawing(cairo_t *cr)
{
  static long k, n = 200;
  static gint count = 0;
  count++;

    for (k=0; k<n; ++k) {
        x[k] = k * dx;
        y[k] = sin(x[k] + 0.1 * count) + sin(1.2 * x[k] - 0.1 * count) ;
    }

    series = slope_xyseries_new_filled("Sine", x, y, n, "b-");
}




static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{      
  do_drawing(cr);  
  return FALSE;
}




static gboolean time_handler(GtkWidget *widget)
{
  if (!timer) return FALSE;

  gtk_widget_queue_draw(widget);
  return TRUE;
}




int main(int argc, char *argv[])
{
    /* number of points */
    long k, n = 200;    
    /*spatial resolution */
    dx = 4.0 * G_PI / n;
    
    
    gtk_init(&argc, &argv);
    chart = slope_chart_new();

    g_signal_connect(G_OBJECT(chart), "draw", 
      G_CALLBACK(on_draw_event), NULL); 
    g_signal_connect(G_OBJECT(chart), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    /* create some sinusoidal data points */

    x = g_malloc(n * sizeof(double));
    y = g_malloc(n * sizeof(double));


    /* the amplitude for the sine wave gives the SCALE of the plot */
    for (k=0; k<n; ++k) {
        x[k] = k * dx;
        y[k] = Max_scale * sin(x[k]);
    }

    scale = slope_xyscale_new();
    slope_chart_add_scale(SLOPE_CHART(chart), scale);

    series = slope_xyseries_new_filled("Sine", x, y, n, "b-");
    slope_scale_add_item(scale, series);
    
    g_timeout_add(15, (GSourceFunc) time_handler, (gpointer) chart);
    gtk_widget_show_all(chart);
    gtk_main();

    g_free(x);
    g_free(y);

    return 0;
}
