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

#include "slope/legend_p.h"
#include <stdlib.h>


slope_legend_t* _slope_legend_create()
{
    
}


void _slope_legend_destroy (slope_legend_t *legend)
{
    
}


void _slope_legend_draw (slope_legend_t *legend, cairo_t *cr,
                         slope_point_t *point)
{
    
}


int slope_legend_visible (slope_legend_t *legend)
{
    if (legend == NULL) {
        return SLOPE_FALSE;
    }
    return legend->visible;
}

/* slope/legend.c */
