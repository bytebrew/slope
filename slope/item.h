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
 * @file slope/item.h
 * @defgroup Item
 * @ingroup Item
 * 
 * @author Elvis Teixeira
 * @date 18 Jan 2015
 * 
 * @brief The base class for item objects.
 * 
 * Everithing that can be drawn in a slope_figure_t, including data
 * representations, chart axis, legends and text labels, are items.
 * To be drawn, items must lie in the item list of metrics that
 * that lie in the metrics list of a figure.
 */

#ifndef SLOPE_DATA_H
#define SLOPE_DATA_H

#include "slope/primitives.h"

SLOPE_BEGIN_DECLS

/**
 * @ingroup Item
 * @brief The symbol to represent a data point in a scatter map type chart
 */
typedef enum
{
    SLOPE_LINE       = 1 << 1, /*!< The data points are joined by lines */
    SLOPE_CIRCLES    = 1 << 2, /*!< The data points are represent by circles, filled or not */
    SLOPE_TRIANGLES  = 1 << 3, /*!< The data points are represent by triangles, filled or not */
    SLOPE_SQUARES    = 1 << 4, /*!< The data points are represent by squares, filled or not */
    SLOPE_PLUSSES    = 1 << 5  /*!< The data points are represent by plus signs */
}
slope_scatter_t;

/**
 * @ingroup Item
 * @brief Destroys an item and frees the memory
 * 
 * @param[in] item The item to be destroyed.
 */
slope_public void
slope_item_destroy (slope_item_t *item);

/**
 * @ingroup Item
 * @brief Queries for the visibility of an item
 * 
 * @returns SLOPE_TRUE if the item is visible, SLOPE_FALSE otherwise
 */
slope_public int
slope_item_get_visible (const slope_item_t *item);

/**
 * @ingroup Item
 * @brief Sets the visibility state of the item
 */
slope_public void
slope_item_toggle_visible (slope_item_t *item, slope_bool_t visible);

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

#endif /* SLOPE_DATA_H */
