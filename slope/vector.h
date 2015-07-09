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

#ifndef SLOPE_VECTOR_H
#define SLOPE_VECTOR_H

#include "slope/global.h"

SLOPE_BEGIN_DECLS

/**
 */
typedef struct _slope_vector
{
    double *v;
    int nalloc;
    int size;
}
slope_vector_t;


slope_public slope_vector_t* slope_vector_create (int size);

slope_public void slope_vector_destroy (slope_vector_t *vector);

slope_public void slope_vector_append (slope_vector_t *vector, double value);

SLOPE_END_DECLS

#endif /* SLOPE_VECTOR_H */
