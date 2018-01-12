/*
 * Copyright (C) 2016  Elvis Teixeira
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
#include <math.h>
#include <slope/slope.h>

/* globals. yes, good or not, it is a demo */
const long n = 100;
double *   x, *y;
GtkWidget *view;

gboolean update_plot(GtkWidget *button, gpointer data)
{
  long k;
  for (k = 0; k < n; ++k)
    {
      x[k] = g_random_double();
      y[k] = g_random_double();
    }
  slope_view_redraw(SLOPE_VIEW(view));
  return TRUE;
}

int main(int argc, char *argv[])
{
  GtkWidget *  window;
  GtkWidget *  box;
  GtkWidget *  button;
  SlopeFigure *figure;
  SlopeScale * scale;
  SlopeItem *  series;

  gtk_init(&argc, &argv);

  /* create window layout */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  box    = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  button = gtk_button_new_with_label("Update plot");
  view   = slope_view_new();
  figure = slope_figure_new();
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
  g_signal_connect(
      G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(update_plot), NULL);
  gtk_container_add(GTK_CONTAINER(window), box);
  gtk_box_pack_start(GTK_BOX(box), view, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), button, FALSE, TRUE, 2);
  slope_view_set_figure(SLOPE_VIEW(view), figure);

  x = g_malloc(n * sizeof(double));
  y = g_malloc(n * sizeof(double));
  update_plot(button, NULL);

  scale = slope_xyscale_new();
  slope_figure_add_scale(SLOPE_FIGURE(figure), scale);

  series = slope_xyseries_new_filled("Random Points", x, y, n, "kor");
  slope_scale_add_item(scale, series);

  gtk_widget_show_all(window);
  gtk_main();

  g_free(x);
  g_free(y);

  return 0;
}
