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
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SLOPE_SERIES_P__
#define __SLOPE_SERIES_P__

#include <slope/series.h>
#include <slope/item_p.h>

#define SLOPE_SERIES_PRIVATE(instance) ((slope_series_private_t*) (instance))
#define SLOPE_SERIES_GET_PRIVATE(instance) SLOPE_SERIES_PRIVATE(SLOPE_ITEM_GET_PRIVATE(instance))

SLOPE_BEGIN_DECLS

typedef struct _slope_series_private slope_series_private_t;

struct _slope_series_private {
   slope_item_private_t parent;

   slope_series_style_t style;
   slope_color_t stroke_color;
   slope_color_t fill_color;
   double line_width;

   const double *vx;
   const double *vy;
   int size;

   double xmin, xmax;
   double ymin, ymax;
   double symbol_radius;
};

SLOPE_END_DECLS

#endif /*__SLOPE_SERIES_P__*/
