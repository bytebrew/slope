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

#ifndef __SLOPE_DATA_H
#define __SLOPE_DATA_H

#include "slope/primitives.h"

__SLOPE_BEGIN_DECLS

/**
 */
typedef enum
{
    SLOPE_LINE,
    SLOPE_CIRCLES,
    SLOPE_TRIANGLES,
    SLOPE_SQUARES,
    SLOPE_PLUSSES
}
slope_scatter_t;

/**
 */
slope_public void
slope_data_destroy (slope_data_t *data);

/**
 */
slope_public int
slope_data_get_visible (slope_data_t *data);

/**
 */
slope_public void
slope_data_set_visible (slope_data_t *data, int visible);

/**
 */
slope_public const char*
slope_data_get_name (slope_data_t *data);

/**
 */
slope_public void
slope_data_set_name (slope_data_t *data, const char *name);

/**
 */
slope_public slope_metrics_t*
slope_data_get_metrics (slope_data_t *data);

/**
 */
slope_public slope_scene_t*
slope_data_get_scene (slope_data_t *data);

__SLOPE_END_DECLS

#endif /*__SLOPE_DATA_H */
