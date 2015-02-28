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

#include "slope-scatter_p.h"
#include <stdlib.h>

slope_scatter_t* slope_scatter_create_for_data (double *vx, double *vy, unsigned long n,
                                                slope_scatter_symbol_t symbol)
{
    slope_scatter_t *scat = malloc(sizeof(slope_scatter_t));
    scat->visib = 1;
    scat->symb = symbol;
    scat->vx = vx;
    scat->vy = vy;
    scat->n = n;
    return scat;
}

void slope_scatter_destroy (slope_scatter_t *scatter)
{
    if (scatter == NULL) {
        return;
    }
    free(scatter);
    scatter = NULL;
}

int slope_scatter_visible (slope_scatter_t *scatter)
{
    return scatter->visib;
}

void _slope_scatter_draw (slope_scatter_t *scatter,
                          slope_plotable_t *cartesian,
                          cairo_t *cr)
{
    //TODO
}

/* slope-scatter.c */