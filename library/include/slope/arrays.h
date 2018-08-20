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

#ifndef _slope_arrays_h_
#define _slope_arrays_h_

#include "slope/drawing.h"

G_BEGIN_DECLS

/*==----------------------------------------------------------------==//
 * 1D number arrays
//==----------------------------------------------------------------==*/

typedef struct _SlopeArray1D SlopeArray1D;
#define SLOPE_ARRAY1D(Addr) ((SlopeArray1D*) (Addr))

SlopeArray1D* slope_array1d_new ();

void slope_array1d_delete (SlopeArray1D *self);

void slope_array1d_append (SlopeArray1D *self, double x);

void slope_array1d_get_points (SlopeArray1D *self,
                               gulong *size,
                               double **points);


/*==----------------------------------------------------------------==//
 * 2D point arrays
//==----------------------------------------------------------------==*/

typedef struct _SlopeArray2D SlopeArray2D;
#define SLOPE_ARRAY2D(Addr) ((SlopeArray2D*) (Addr))

SlopeArray2D* slope_array2d_new ();

void slope_array2d_delete (SlopeArray2D *self);

void slope_array2d_append (SlopeArray2D *self, double x, double y);

void slope_array2d_get_points (SlopeArray2D *self,
                               gulong *size,
                               SlopePoint **points);

G_END_DECLS

#endif /* _slope_arrays_h_ */
