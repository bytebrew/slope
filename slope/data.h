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

#ifndef _SLOPE_DATA_H_
#define _SLOPE_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _slope_data slope_data_t;


void slope_data_destroy (slope_data_t *data);

int slope_data_visible (slope_data_t *data);

const char* slope_data_name (slope_data_t *data);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_XYMETRICS_H_*/
