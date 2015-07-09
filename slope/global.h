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

#ifndef SLOPE_GLOBAL_H
#define SLOPE_GLOBAL_H

/**
 */
#ifdef _cplusplus
# define SLOPE_BEGIN_DECLS  extern "C" {
# define SLOPE_END_DECLS    }
#else
# define SLOPE_BEGIN_DECLS
# define SLOPE_END_DECLS
#endif

/**
 */
#if defined (_MSC_VER)
# if defined (SLOPE_BUILD)
#  define slope_public _declspec(dllexport)
# else
#  define slope_public _declspec(dllimport)
# endif
#else
#  define slope_public
#endif

#endif /*SLOPE_GLOBAL_H */
