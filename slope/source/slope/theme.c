/*
 * Copyright (C) 2018 Elvis M. Teixeira
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "slope/theme.h"

SlopeTheme _default_themes_array[] = {
    {
        {
            SLOPE_RED,
            SLOPE_GREEN,
            SLOPE_BLUE,
            SLOPE_BLACK
        },
        {
            1.0, 1.0, 1.0, 1.0
        }
    },
    {
        {
            SLOPE_BLUE,
            SLOPE_GREEN,
            SLOPE_RED,
            SLOPE_BLACK
        },
        {
            1.0, 1.0, 1.0, 1.0
        }
    },
    {
        {
            SLOPE_RED,
            SLOPE_BLUE,
            SLOPE_GREEN,
            SLOPE_BLACK
        },
        {
            1.0, 1.0, 1.0, 1.0
        }
    }
};

SlopeTheme const *const slope_default_item_themes = _default_themes_array;
SlopeTheme const *const slope_default_frame_themes = _default_themes_array;
