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

#ifndef SLOPE_THEME_H
#define SLOPE_THEME_H

#include "slope/color.h"

typedef struct _SlopeTheme SlopeTheme;

struct _SlopeTheme {
    SlopeColor colors[4];
    double line_widths[4];
};


extern SlopeTheme const *const slope_default_item_themes;
extern SlopeTheme const *const slope_default_frame_themes;

#endif /* SLOPE_THEME_H */
