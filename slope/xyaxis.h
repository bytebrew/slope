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

#ifndef SLOPE_XYAXIS_H
#define SLOPE_XYAXIS_H

#include "slope/item.h"

SLOPE_BEGIN_DECLS

/**
 */
typedef enum _slope_xyaxis_type
{
    SLOPE_XYAXIS_TOP     = 0,
    SLOPE_XYAXIS_BOTTOM  = 1,
    SLOPE_XYAXIS_LEFT    = 2,
    SLOPE_XYAXIS_RIGHT   = 3
}
slope_xyaxis_type_t;

/**
 */
slope_public slope_item_t*
slope_xyaxis_create (slope_metrics_t *metrics,
                     slope_xyaxis_type_t type,
                     const char *name);

/**
 */
slope_public slope_xyaxis_type_t
slope_xyaxis_get_type (slope_item_t *axis);

SLOPE_END_DECLS

#endif //SLOPE_XYAXIS_H
