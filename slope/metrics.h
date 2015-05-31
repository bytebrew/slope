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

#ifndef __SLOPE_METRICS_H
#define __SLOPE_METRICS_H

#include "slope/list.h"
#include "slope/primitives.h"

__SLOPE_BEGIN_DECLS

/**
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
                           int visible);

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
slope_public slope_list_t*
slope_metrics_get_item_list (const slope_metrics_t *metrics);

/**
 */
slope_public slope_figure_t*
slope_metrics_get_figure (const slope_metrics_t *metrics);

__SLOPE_END_DECLS

#endif /*__SLOPE_METRICS_H */
