/*
 * Copyright (C) 2018 Elvis M. Teixeira
 *   
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef SLOPE_COLOR_H
#define SLOPE_COLOR_H

#include "slope/global.h"

typedef unsigned int SlopeColor;

#define SLOPE_BLACK      0x000000ff
#define SLOPE_WHITE      0xffffffff
#define SLOPE_RED        0xff0000ff
#define SLOPE_GREEN      0x00ff00ff
#define SLOPE_BLUE       0x0000ffff

#define slope_get_red(C)      slope_last_n_bits((C) >> 24, 8)
#define slope_get_green(C)    slope_last_n_bits((C) >> 16, 8)
#define slope_get_blue(C)     slope_last_n_bits((C) >>  8, 8)
#define slope_get_alpha(C)    slope_last_n_bits((C)      , 8)

#define slope_set_red(C,V)      C = slope_set_char(C, 24, V)
#define slope_set_green(C,V)    C = slope_set_char(C, 16, V)
#define slope_set_blue(C,V)     C = slope_set_char(C,  8, V)
#define slope_set_alpha(C,V)    C = slope_set_char(C,  0, V)

#endif /* SLOPE_COLOR_H */
