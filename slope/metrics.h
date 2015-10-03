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
 * @file slope/metrics.h
 * @defgroup Metrics
 * @ingroup Metrics
 * 
 * @author Elvis Teixeira
 * @date 18 Jan 2015
 * 
 * @brief The base class for metrics objects.
 * 
 * Metrics are used to scale the datasets to the size of the figure and to
 * help place and anchor the various items to the appropriate figure layout.
 */


#ifndef SLOPE_METRICS_H
#define SLOPE_METRICS_H

#include "slope/list.h"
#include "slope/object.h"
#include "slope/primitives.h"

SLOPE_BEGIN_DECLS

/**
 * @ingroup Metrics
 * 
 * @brief The metrics type tells the type of transformation from data space to figure coordinates
 * 
 * The metrics type tells the type of transformation from data space to figure coordinates,
 * the only transformation implemented at this time is slope_xymetrics_t, which performs the
 * transformation from data in cartesian coordinates to cartesian figure coordinates.
 */
typedef enum _slope_metrics_type
{
    SLOPE_METRICS_INVALID = 0, /*!< Uninitialised metrics object */
    SLOPE_XYMETRICS       = 1  /*!< XY or cartesian metrics object */
}
slope_metrics_type_t;


struct _slope_metrics
{
    slope_object_t parent;
};


/**
 * @ingroup Metrics
 * @brief Destroys a metrics object and frees the memory.
 * 
 * @param[in] metrics The metrics to be destroyed.
 */
slope_public void
slope_metrics_destroy (slope_metrics_t *metrics);

/**
 */
slope_public int
slope_metrics_get_visible (const slope_metrics_t *metrics);

/**
 */
slope_public void
slope_metrics_set_visible (slope_metrics_t *metrics,
                           slope_bool_t visible);

/**
 */
slope_public slope_metrics_type_t
slope_metrics_get_type (const slope_metrics_t *metrics);

/**
 */
slope_public void
slope_metrics_update (slope_metrics_t *metrics);

/**
 */
slope_public void
slope_metrics_add_item (slope_metrics_t *metrics,
                        slope_item_t *item);

/**
 */
slope_public void
slope_metrics_remove_item (slope_metrics_t *metrics,
                           slope_item_t *item);

/**
 */
slope_public slope_list_t*
slope_metrics_get_item_list (const slope_metrics_t *metrics);

/**
 */
slope_public slope_figure_t*
slope_metrics_get_figure (const slope_metrics_t *metrics);

/**
 */
slope_public void
slope_metrics_get_data_rect (const slope_metrics_t *metrics,
                             slope_rect_t *rect);

/**
 */
slope_public void
slope_metrics_get_figure_rect (const slope_metrics_t *metrics,
                               slope_rect_t *rect);

SLOPE_END_DECLS

#endif /* SLOPE_METRICS_H */
