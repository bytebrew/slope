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

#ifndef slope_global_h
#define slope_global_h

typedef double           slope_float_t;
typedef int              slope_int32_t;
typedef unsigned int     slope_uint32_t;
typedef long             slope_int64_t;
typedef unsigned long    slope_uint64_t;
typedef char             slope_bool_t;

#define SLOPE_TRUE  ((slope_bool_t) (1==1))
#define SLOPE_FALSE ((slope_bool_t) (1==0))
#define SLOPE_NULL  ((void *) 0)

#define slope_enabled(flags, bit) \
  (((flags) & (bit)) == (bit))

#define slope_enable_if(flags, bit, yes_no) \
  flags = (yes_no) ? (flags | bit) : (flags & ~(bit))

#define slope_enable(flags, bit) \
  flags |= (bit)

#define slope_disable(flags, bit) \
  flags &= ~(bit)

#endif /* slope_global_h */
