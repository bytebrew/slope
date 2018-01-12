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

#include <math.h>
#include <slope/slope.h>

int main(int argc, char *argv[])
{
  GtkWidget *   chart;
  SlopeScale *  scale1, *scale2;
  SlopeItem *   series11, *series12, *series2;
  SlopeItem *   axis;
  SlopeSampler *sampler;
  double *      x, *y11, *y12, *y2;

  gtk_init(&argc, &argv);
  chart = slope_chart_new();

  g_signal_connect(G_OBJECT(chart), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* create some sinusoidal data points */
  long k, n = 50;
  x         = g_malloc(n * sizeof(double));
  y11       = g_malloc(n * sizeof(double));
  y12       = g_malloc(n * sizeof(double));
  y2        = g_malloc(n * sizeof(double));
  double dx = 4.0 * G_PI / n;

  for (k = 0; k < n; ++k)
    {
      x[k]   = k * dx;
      y11[k] = sin(x[k]);
      y12[k] = cos(x[k]);
      y2[k]  = 1.0 + y11[k] + 0.1 * k;
    }

  scale1 = slope_xyscale_new_axis("Phase", "Amplitude", "Sinusoidal functions");
  slope_scale_set_layout_rect(scale1, 0, 0, 1, 1);
  slope_chart_add_scale(SLOPE_CHART(chart), scale1);
  axis =
      slope_xyscale_get_axis(SLOPE_XYSCALE(scale1), SLOPE_XYSCALE_AXIS_BOTTOM);
  sampler = slope_xyaxis_get_sampler(SLOPE_XYAXIS(axis));
  slope_sampler_set_samples(sampler, slope_sampler_pi_samples, 9);

  scale2 = slope_xyscale_new_axis("Month", "Value", "Projected profit");
  slope_scale_set_layout_rect(scale2, 0, 1, 1, 1);
  slope_chart_add_scale(SLOPE_CHART(chart), scale2);
  axis =
      slope_xyscale_get_axis(SLOPE_XYSCALE(scale2), SLOPE_XYSCALE_AXIS_BOTTOM);
  sampler = slope_xyaxis_get_sampler(SLOPE_XYAXIS(axis));
  slope_sampler_set_samples(sampler, slope_sampler_month_samples, 12);

  series11 = slope_xyseries_new_filled("Sine", x, y11, n, "r-");
  slope_scale_add_item(scale1, series11);
  series12 = slope_xyseries_new_filled("Cossine", x, y12, n, "ga");
  slope_scale_add_item(scale1, series12);

  series2 = slope_xyseries_new_filled("Sine + Linear", x, y2, n, "kob");
  slope_scale_add_item(scale2, series2);

  gtk_widget_show_all(chart);
  gtk_main();

  g_free(x);
  g_free(y11);
  g_free(y12);
  g_free(y2);

  return 0;
}
