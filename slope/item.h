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

/**
 * @file item.h
 * @brief The base class for item objects
 * 
 * @defgroup Item
 */

#ifndef SLOPE_DATA_H
#define SLOPE_DATA_H

#include "slope/primitives.h"

SLOPE_BEGIN_DECLS

/**
 */
typedef enum
{
    SLOPE_LINE       = 0,
    SLOPE_CIRCLES    = 1,
    SLOPE_TRIANGLES  = 2,
    SLOPE_SQUARES    = 3,
    SLOPE_PLUSSES    = 4
}
slope_scatter_t;

/**
 */
slope_public void
slope_item_destroy (slope_item_t *item);

/**
 */
slope_public int
slope_item_get_visible (const slope_item_t *item);

/**
 */
slope_public void
slope_item_set_visible (slope_item_t *item, int visible);

/**
 */
slope_public const char*
slope_item_get_name (const slope_item_t *item);

/**
 */
slope_public void
slope_item_set_name (slope_item_t *item, const char *name);

/**
 */
int slope_item_get_has_thumb (const slope_item_t *item);

/**
 */
slope_public slope_metrics_t*
slope_item_get_metrics (const slope_item_t *item);

/**
 */
slope_public slope_figure_t*
slope_item_get_figure (const slope_item_t *item);

/**
 */
slope_public void
slope_item_notify_appearence_change (slope_item_t *item);

/**
 */
slope_public void
slope_item_notify_data_change (slope_item_t *item);

SLOPE_END_DECLS

#endif /*SLOPE_DATA_H */
