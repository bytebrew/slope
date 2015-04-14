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

#ifndef __SLOPE_XYDATA_H
#define __SLOPE_XYDATA_H

#include "slope/data.h"

__SLOPE_BEGIN_DECLS

/**
 */
slope_public slope_data_t* slope_xydata_create ();

/**
 */
slope_public slope_data_t*
slope_xydata_create_simple (const double *vx, const double *vy,
                            const int n,
                            const char *name,
                            slope_color_name_t color,
                            slope_scatter_t scatter);

/**
 */
slope_public void
slope_xydata_set (slope_data_t *data,
                  const double *vx, const double *vy,
                  const int n,
                  const char *name,
                  slope_color_name_t color,
                  slope_scatter_t scatter);

/**
 */
slope_public void
slope_xydata_set_data (slope_data_t *data,
                       const double *vx, const double *vy,
                       const int n);

/**
 */
slope_public void
slope_xydata_update_data (slope_data_t *data,
                          const double *vx, const double *vy,
                          const int n);

__SLOPE_END_DECLS

#endif /*__SLOPE_XYDATA_H */
