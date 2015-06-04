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

#ifndef SLOPE_LEGEND_H
#define SLOPE_LEGEND_H

#include "slope/item.h"

SLOPE_BEGIN_DECLS

typedef enum _slope_legend_position
{
    SLOPE_LEGEND_TOPRIGHT,
    SLOPE_LEGEND_TOPLEFT,
    SLOPE_LEGEND_TOPRIGHT_OUTSIDE,
    SLOPE_LEGEND_TOPLEFT_OUTSIDE
}
slope_legend_position_t;


slope_public slope_item_t* slope_legend_create ();

SLOPE_END_DECLS

#endif /*SLOPE_LEGEND_H */
