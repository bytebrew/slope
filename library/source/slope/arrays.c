/*
 * Copyright (C) 2019  Elvis Teixeira
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

typedef enum {
    VECTOR_IS_VIEW   = 1U,

    VECTOR_FLOAT_1D  = 1U << 1U,
    VECTOR_FLOAT_2D  = 1U << 2U,
    VECTOR_FLOAT_3D  = 1U << 3U,

} VectorTraits;


typedef struct _Vector {
    gpointer data;
    gulong size;
    gulong capacity;
    gulong element_size;
    gulong traits;
} Vector;


static Vector *
vector_new (gulong element_size)
{
    Vector *self;

    self = g_new (Vector, 1);
    self->data = NULL;
    self->size = 0L;
    self->capacity = 0L;
    self->element_size = element_size;

    return self;
}


static void
vector_delete (Vector *self)
{
    if (self->data != NULL && self->capacity > 0L) {
        g_free (self->data);
    }

    g_free (self);
}


static void
vector_grow (Vector *self)
{
    if (self->capacity == 0UL) {
        self->capacity = 32UL;
        self->data = g_malloc (self->capacity * self->element_size);
    } else {
        self->capacity <<= 1UL;
        self->data = g_realloc (self->data, self->capacity * self->element_size);
    }
}


/*==----------------------------------------------------------------==//
 * 1D number arrays
//==----------------------------------------------------------------==*/

SlopeArray1D* slope_array1d_new ()
{
    Vector *self = vector_new (sizeof (double));
    self->traits = VECTOR_FLOAT_1D;
    return  (SlopeArray1D*) self;
}


void slope_array1d_delete (SlopeArray1D *self)
{
    vector_delete ((Vector *) self);
}


void slope_array1d_append (SlopeArray1D *self, double x)
{
    Vector *vec;
    double *pts;

    g_return_if_fail (self != NULL);

    vec = (Vector *) self;

    if (vec->size == vec->capacity) {
        vector_grow (vec);
    }

    pts = (double *) vec->data;
    pts[vec->size] = x;
    vec->size += 1L;
}


void slope_array1d_get_points (SlopeArray1D *self,
                               gulong *size,
                               double **points)
{
    Vector *vec;

    g_return_if_fail (self != NULL);

    vec = (Vector *) self;

    *size = vec->size;
    *points = (double *) vec->data;
}

/*==----------------------------------------------------------------==//
 * 2D point arrays
//==----------------------------------------------------------------==*/

SlopeArray2D*
slope_array2d_new ()
{
    Vector *self = vector_new (sizeof (SlopePoint));
    self->traits = VECTOR_FLOAT_2D;
    return  (SlopeArray2D*) self;
}


void
slope_array2d_delete (SlopeArray2D *self)
{
    vector_delete ((Vector *) self);
}


void
slope_array2d_append (SlopeArray2D *self, double x, double y)
{
    Vector *vec;
    SlopePoint *pts;

    g_return_if_fail (self != NULL);

    vec = (Vector *) self;

    if (vec->size == vec->capacity) {
        vector_grow (vec);
    }

    pts = (SlopePoint *) vec->data;
    pts[vec->size].x = x;
    pts[vec->size].y = y;
    vec->size += 1L;
}


void slope_array2d_get_points (SlopeArray2D *self,
                               gulong *size,
                               SlopePoint **points)
{
    Vector *vec;

    g_return_if_fail (self != NULL);

    vec = (Vector *) self;

    *size = vec->size;
    *points = (SlopePoint *) vec->data;
}

/* slope/arrays.c */
