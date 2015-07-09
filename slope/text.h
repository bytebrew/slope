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
 * @file
 * @defgroup Text
 * @ingroup Text
 * 
 * @brief Functions to handle backend neutral font and text handling
 * and rendering
 */

#ifndef _SLOPE_TEXT_H_
#define _SLOPE_TEXT_H_

#include "slope/primitives.h"

SLOPE_BEGIN_DECLS

/**
 * @ingroup Text
 * @brief A font descriptor interface for cairo toy api or pango
 */
typedef struct _slope_font slope_font_t;


/**
 * @ingroup Text
 * @brief Creates a new font desciption object
 */
slope_public slope_font_t*
slope_font_create (const char *family, int size);

/**
 * @ingroup Text
 * @brief Sets the family and size of a font object
 */
slope_public void
slope_font_select (slope_font_t *font,
                   const char *family, int size);

/**
 * @ingroup Text
 * @brief Destroys a font desciption object
 */
slope_public void
slope_font_destroy (slope_font_t *font);

/**
 * @ingroup Text
 * @brief Draws a string of text in a item context
 */
slope_public void
slope_draw_text (cairo_t *cr, slope_font_t *font,
                 double x, double y, const char *text);

/**
 * @ingroup Text
 * @brief Retrieves the width and height of a text string as shown in a figure
 * 
 * @param[in]  cr    The cairo context that will draw the text
 * @param[in]  font  The font in wich the txt is to be drawn
 * @param[out] rect  The rectangle in witch the extents of the text will be put
 * @param[in]  tect  The string whose extents are to be retrieved
 */
slope_public void
slope_get_text_rect (cairo_t *cr, slope_font_t *font,
                     slope_rect_t *rect, const char *text);

SLOPE_END_DECLS 

#endif /*_SLOPE_TEXT_H_*/
