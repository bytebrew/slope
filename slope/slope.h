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

/**
 * @file slope/slope.h
 * @defgroup Util
 * @ingroup Util
 * 
 * @author Elvis Teixeira
 * @date 18 Jan 2015
 * 
 * @brief Functions to quick and easy usage.
 * 
 * The functions in this module are provided by convenience
 * to permit fast creation of most commom chart formats as well
 * as showing them in a Window if slope was build with gtk support.
 */

/**
 * @mainpage Slope, a free charting library that uses cairo
 * 
 * Slope is developed with the goal of providing an easy way to
 * cairo and Gtk users to generate charts to visualize numerical
 * data. The coding conventions are similar to those used in cairo.
 * 
 * Slope's only mandatory dependency is cairo, if you turn off the
 * build flag that includes Gtk. The Gtk dependent build has the
 * advantage of using the pango text layout library that is much
 * more powerful than the cairo "toy" API for text rendering, and of
 * course the ability to add a custom widget to your Gtk GUI with
 * mouse zooming enabled.
 */

#ifndef SLOPE_H
#define SLOPE_H

/* for figure object */
#include "slope/figure.h"
/* for xy charts */
#include "slope/xymetrics.h"
#include "slope/xyitem.h"
#include "slope/xyaxis.h"


/**
 * @ingroup Util
 * @brief Creates a cartesian (XY) chart to show functional data
 * 
 * @param[in] title    The title to be shown above the chart
 * @param[in] xlabel   The text label to X axis
 * @param[in] ylabel   The text label to Y axis
 * 
 * @return Your new chart as a figure object
 */
slope_public slope_figure_t*
slope_chart_create (const char *title,
                    const char *xlabel,
                    const char *ylabel);

slope_public void
slope_chart_destroy (slope_figure_t *figure);

slope_public slope_item_t*
slope_chart_add_plot (slope_figure_t *chart,
                      const double *x, const double *y, int n,
                      const char *title, const char *fmt);

#endif /*SLOPE_H */
