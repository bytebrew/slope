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

#ifndef __SLOPE_SCALE_P__
#define __SLOPE_SCALE_P__

#include <slope/scale.h>

#define SLOPE_SCALE_PRIVATE(instance) ((slope_scale_private_t*) (instance))
#define SLOPE_SCALE_GET_PRIVATE(instance) SLOPE_SCALE_PRIVATE(SLOPE_SCALE(instance)->_private)

SLOPE_BEGIN_DECLS

typedef struct _slope_scale_private slope_scale_private_t;

struct _slope_scale_private {
    slope_figure_t *figure;
   slope_list_t *item_list;
   slope_bool_t visible;
   char *name;
};


void _slope_scale_draw (slope_scale_t *self, const slope_rect_t *rect, cairo_t *cr);
void _slope_scale_set_figure (slope_scale_t *self, slope_figure_t *figure);

SLOPE_END_DECLS

#endif /*__SLOPE_SCALE_P__*/
