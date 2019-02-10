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

#include "slope/global.h"

#define ALMOST_ZERO 1.0e-10

gboolean slope_float_similar_zero(double x)
{
    x = (x > 0.0) ? x : -x;
    return x < ALMOST_ZERO;
}


gboolean slope_float_similar(double x, double y)
{
    x = x - y;
    x = (x > 0.0) ? x : -x;
    return x < ALMOST_ZERO;
}
