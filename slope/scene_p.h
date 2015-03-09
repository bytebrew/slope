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

#ifndef _SLOPE_SCENE_P_H_
#define _SLOPE_SCENE_P_H_

#include "slope/scene.h"
#include "slope/list.h"
#include "slope/primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _slope_scene
{
    slope_list_t *metrics;
    slope_color_t back_color;
    int fill_back;

    void (*_cleanup_fn) (slope_scene_t*);
};

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_SCENE_P_H_*/