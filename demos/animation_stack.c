/*
 * Copyright (C) 2017  Nuno Ferreira
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

#include <gtk/gtk.h>
#include <slope/slope.h>
#include <math.h>

const long n = 100;
double *x, *y_1, *y_2;
double dx;

GtkWidget *view_1;
GtkWidget *view_2;

SlopeFigure *figure;
SlopeFigure *figure_2;

SlopeItem *series;
SlopeItem *series_2;

gboolean timer = TRUE;
#define Max_scale 2.5 



static void update_calculations(cairo_t *cr) {
    static long k, n = 200;
    static gint count = 0;
    count++;
    for (k=0; k<n; ++k) {
        x[k] = k * dx;
        y_1[k] = Max_scale * sin(x[k] - 0.1 * count);
        y_2[k] = Max_scale * cos(1.5 * x[k] + 0.1 * count);
    }
}


static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    update_calculations(cr);
    return FALSE;
}



static gboolean time_handler(GtkWidget *widget) {
  if (!timer) return FALSE;
  gtk_widget_queue_draw(widget);
  return TRUE;
}





int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *button;
    
    SlopeScale *scale;
    SlopeScale *scale_2;

    gtk_init(&argc, &argv);

    /* create window layout */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    button = gtk_button_new_with_label("quit");
    
    view_1 = slope_view_new();
    view_2 = slope_view_new();
    
    figure = slope_figure_new();
    figure_2 = slope_figure_new();
    
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_box_pack_start(GTK_BOX(box), view_1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), view_2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, TRUE, 2);
    slope_view_set_figure(SLOPE_VIEW(view_1), figure);
    slope_view_set_figure(SLOPE_VIEW(view_2), figure_2);
    
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);                 
    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(on_draw_event), NULL);
    //g_signal_connect(G_OBJECT(view_2), "draw", G_CALLBACK(on_draw_event), NULL);
    

    
    /* number of points */
    long k, n = 200;
    /*spatial resolution */
    dx = 4.0 * G_PI / n;

    x = g_malloc(n * sizeof(double));
    y_1 = g_malloc(n * sizeof(double));
    y_2 = g_malloc(n * sizeof(double));
    
    // y2 = g_malloc(n * sizeof(double));    
    /* the amplitude for the sine wave gives the SCALE of the plot */
    for (k=0; k<n; ++k) {
        x[k] = k * dx;
        y_1[k] = Max_scale * sin(x[k]);
        y_2[k] = Max_scale * cos(1.5 * x[k]);
    }
    
    slope_view_redraw(SLOPE_VIEW(view_1));
    slope_view_redraw(SLOPE_VIEW(view_2));
    
    scale = slope_xyscale_new();
    scale_2 = slope_xyscale_new();
    slope_figure_add_scale(SLOPE_FIGURE(figure), scale);
    slope_figure_add_scale(SLOPE_FIGURE(figure_2), scale_2);

    series = slope_xyseries_new_filled("Wave 1", x, y_1, n, "b-");
    series_2 = slope_xyseries_new_filled("Wave 2", x, y_2, n, "r-");
    slope_scale_add_item(scale, series);
    slope_scale_add_item(scale_2, series_2);

    
    g_timeout_add(30, (GSourceFunc) time_handler, (gpointer) window);
    
    gtk_window_set_title(GTK_WINDOW(window), "animation_stack");
    gtk_widget_show_all(window);
    gtk_main();

    g_free(x);
    g_free(y_1);
    g_free(y_2);
    
    return 0;
}
