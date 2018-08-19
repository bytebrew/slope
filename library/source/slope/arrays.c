/*
 * Copyright (C) 2018  Elvis Teixeira
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

#include "slope/arrays.h"


struct _SlopeArray2D {
    SlopePoint *points;
    unsigned long size;
    unsigned long capacity;
};


SlopeArray2D *slope_array2d_new(unsigned long capacity)
{
    SlopeArray2D *self = g_malloc (sizeof (SlopeArray2D));

    self->points = NULL;
    self->size = 0L;
    self->capacity = capacity;

    if (capacity > 0L) {
        self->points = g_malloc (self->capacity * sizeof (SlopePoint));
    }

    return self;
}


void slope_array2d_delete (SlopeArray2D *self)
{
    if (self->points) {
        g_free (self->points);
    }
    g_free (self);
}


void slope_array2d_append (SlopeArray2D *self, double x, double y)
{
    if (self->size == self->capacity) {
        if (self->points != NULL) {
            self->capacity <<= 1L;
            self->points = g_realloc (self->points, self->capacity * sizeof (SlopePoint));
        } else {
            self->capacity = 10L;
            self->points = g_malloc (self->capacity * sizeof (SlopePoint));
        }
    }

    self->points[self->size].x = x;
    self->points[self->size].y = y;
    self->size += 1L;
}


void slope_array2d_get_points (SlopeArray2D *self,
                               unsigned long *size,
                               SlopePoint **points)
{
    *size = self->size;
    *points = self->points;
}

/* slope/arrays.c */
