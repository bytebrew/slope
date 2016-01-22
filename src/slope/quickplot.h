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

#ifndef __SLOPE_QUICKPLOT__
#define __SLOPE_QUICKPLOT__

#include <slope/figure.h>
#include <slope/linear.h>
#include <slope/axis.h>
#include <slope/legend.h>
#include <slope/series.h>

/* TODO */

SLOPE_BEGIN_DECLS

typedef struct _slope_quickplot slope_quickplot_t;

slope_quickplot_t* slope_quickplot_new();
void slope_quickplot_destroy(slope_quickplot_t *self);

SLOPE_END_DECLS

#endif /*__SLOPE_QUICKPLOT__*/
