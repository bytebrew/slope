/*
 * Copyright (C) 2015  Elvis Teixeira
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
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SLOPE_H
#define __SLOPE_H

/* for figure object */
#include "slope/figure.h"
/* for xy charts */
#include "slope/xymetrics.h"
#include "slope/xyitem.h"
#include "slope/xyaxis.h"


/*
 * the functions bellow are convenient shortcuts to create commom
 * figures and plots
 */

slope_public slope_figure_t*
slope_chart_create (const char *title,
                    const char *xlabel,
                    const char *label);

slope_public void
slope_chart_destroy (slope_figure_t *figure);

slope_public void
slope_chart_add_plot (slope_figure_t *chart,
                      const double *x, const double *y, int n,
                      const char *title, int line, const char *fmt);

#endif /*__SLOPE_H */

