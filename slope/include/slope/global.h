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

#ifndef SLOPE_GLOBAL_H
#define SLOPE_GLOBAL_H

#include <glib.h>

#ifdef __cplusplus
#  define SLOPE_BEGIN_DECLS extern "C" {
#  define SLOPE_END_DECLS }
#else
#  define SLOPE_BEGIN_DECLS
#  define SLOPE_END_DECLS
#endif

/*! @brief Checks is a bit is set */
#define slope_enabled(X,B) (((X) & (B)) == (B))

/*! @brief Turns a bit on */
#define slope_enable(X,B) X |= (B)

/*! @brief Turns a bit off */
#define slope_disable(X,B) X &= ~(B)

/*! @brief Expands to the maximum N bit number */
#define slope_max_n_bits(N) ( ~(0xffffffffU << (N)) )

/*! @brief Expands to X truncated to N bits */
#define slope_last_n_bits(X,N)  ((X) & slope_max_n_bits(N))

/*! @brief Gets the value of the nth character */
#define slope_select_char(X,N)  \
    ((X) & (0xff << (N)))

/*! @brief Clears the nth character */
#define slope_clear_char(X,N)  \
    ((X) & ( ~(0xff << (N)) ))

/*! @brief Sets the nth char in number X */
#define slope_set_char(X,N,C)  \
    (slope_clear_char(X,N) | (((gint)(C)) << (N)))

#endif /* SLOPE_GLOBAL_H */
