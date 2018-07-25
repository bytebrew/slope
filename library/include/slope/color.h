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

#ifndef SLP_COLOR_H
#define SLP_COLOR_H

#include "slope/global.h"

typedef guint32 SlpColor;

#define SLP_BLACK 0x000000FF
#define SLP_WHITE 0xFFFFFFFF
#define SLP_RED   0xFF0000FF
#define SLP_GREEN 0x00FF00FF
#define SLP_BLUE  0x0000FFFF

#define slp_get_red(C)    (((C) >> 24U) & 0xFF)
#define slp_get_green(C)  (((C) >> 16U) & 0xFF)
#define slp_get_blue(C)   (((C) >> 8U) & 0xFF)
#define slp_get_alpha(C)  ((C) & 0xFF)

#endif /* SLP_COLOR_H */
