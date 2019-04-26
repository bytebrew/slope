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

#ifndef SLOPE_GLOBAL_H
#define SLOPE_GLOBAL_H

#if !defined (SLOPE_H_INSIDE) && !defined (SLOPE_COMPILATION)
#error "Only <slope.h> can be included directly."
#endif

#include <glib.h>

#define slope_set(X,b)           X |= (b)
#define slope_clear(X,b)         X &= ~(b)
#define slope_set_nth(X,n)       X |= ((1UL) << (n))
#define slope_clear_nth(X,n)     X &= ~((1UL) << (n))
#define slope_enabled(X,n)       (((X) & (n)) == (n))
#define slope_enabled_any(X,n)   (((X) & (n)) != 0)

G_BEGIN_DECLS

guint32 slope_version_number();

G_END_DECLS

#endif /* SLOPE_GLOBAL_H */
