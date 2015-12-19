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

#ifndef __SLOPE_FIGURE_P__
#define __SLOPE_FIGURE_P__

#include <slope/figure.h>

#define SLOPE_FIGURE_PRIVATE(instance) ((slope_figure_private_t*) (instance))
#define SLOPE_FIGURE_GET_PRIVATE(instance) SLOPE_FIGURE_PRIVATE(SLOPE_FIGURE(instance)->_private)

SLOPE_BEGIN_DECLS

typedef struct _slope_figure_private slope_figure_private_t;

struct _slope_figure_private {
   slope_list_t *scale_list;
   slope_item_t *legend;
   slope_scale_t *ref_scale;
   slope_color_t back_color;
   slope_color_t name_color;
   slope_bool_t show_title;
   slope_rect_t rect;
   char *name;
};


SLOPE_END_DECLS

#endif /*__SLOPE_FIGURE_P__*/
