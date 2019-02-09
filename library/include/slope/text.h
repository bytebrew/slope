/*
 * Copyright (C) 2019  Elvis Teixeira
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
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _slope_text_h_
#define _slope_text_h_

#include "slope/drawing.h"
#include <pango/pangocairo.h>

G_BEGIN_DECLS

/*! \brief Holds text layout and style infor for slope items */
typedef struct _SlopeText SlopeText;
#define SLOPE_TEXT(Addr) ((SlopeText *) (Addr))

struct _SlopeText {
    PangoFontDescription *font_desc;
    PangoLayout *layout;
    cairo_t *cr;
};


SlopeText* slope_text_new (const char *font_desc);

void slope_text_init (SlopeText *self, cairo_t *cr);

void slope_text_delete (SlopeText *self);

int slope_text_set (SlopeText *self, const char *text);

int slope_text_show (SlopeText *self);

int slope_text_get_extents (SlopeText *self, SlopeRect *ink_rect, SlopeRect *logical_rect);

G_END_DECLS

#endif /* _slope_text_h_ */
