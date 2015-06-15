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

#ifndef SLOPE_PRIMITIVES_H
#define SLOPE_PRIMITIVES_H

#include "slope/global.h"

#include "slope-config.h"
#include <cairo.h>
#if SLOPE_HAVE_PANGO
# include <pango/pangocairo.h>
#endif

SLOPE_BEGIN_DECLS

/**
 * @file primitives.h
 * @defgroup Primitives
 * 
 * @ingroup Primitives
 * @brief Some primitive geometric and visible objects like points and colors
 */


/**
 * @ingroup Figure
 * @brief The figure type represents the final drawable of slopes architecture
 * This is the final product os a user operation, it can dre drawn in any cairo backend
 * like GtkWidgets or one of many graphics file formats
 */
typedef struct _slope_figure slope_figure_t;


/**
 * @ingroup Metrics
 * @brief Metrics objects scale the data representations to the fiure's rect and helps items
 * to place themselves around
 */
typedef struct _slope_metrics slope_metrics_t;


/**
 * @ingroup Item
 * @brief Every thing that can be drawn to a figure is a subclass of slope_item_t
 */
typedef struct _slope_item slope_item_t;


/**
 */
typedef void (*slope_callback_t) (slope_figure_t*);


/**
 * @ingroup Primitives
 * @brief The boolean type
 */
typedef unsigned char slope_bool_t;


/**
 * @ingroup Primitives
 * @brief The boolean false value
 */
#define SLOPE_FALSE 0


/**
 * @ingroup Primitives
 * @brief The boolean true value
 */
#define SLOPE_TRUE  1


/**
 * @ingroup Primitives
 * @brief A font descriptor interface for cairo toy api or pango
 */
typedef struct _slope_font
{
#if SLOPE_HAVE_PANGO
    PangoFontDescription *font;
#else
    cairo_scaled_font_t *font;
#endif
}
slope_font_t;


/**
 */
typedef struct _slope_point
{
    double x;
    double y;
}
slope_point_t;


/**
 */
typedef struct _slope_rect
{
    double x;
    double y;
    double width;
    double height;
}
slope_rect_t;


/**
 */
typedef enum _slope_status
{
    SLOPE_SUCCESS = 0,
    SLOPE_ERROR = 1
}
slope_status_t;


/**
 */
typedef enum _slope_color_name
{
    SLOPE_BLACK          = 0,
    SLOPE_WHITE          = 1,
    SLOPE_RED            = 2,
    SLOPE_GREEN          = 3,
    SLOPE_BLUE           = 4,
    SLOPE_MAROON         = 5,
    SLOPE_PURPLE         = 6,
    SLOPE_YELLOW         = 7,
    SLOPE_GREY           = 8,
    SLOPE_OLIVE          = 9,
    SLOPE_ORANGE         = 10,
    SLOPE_TEAL           = 11,
    SLOPE_LAST_COLOR     = 12
}
slope_color_name_t;


/**
 */
typedef enum _slope_paper_size
{
    SLOPE_PAPER_SIZE_A0      = 0,
    SLOPE_PAPER_SIZE_A1      = 1,
    SLOPE_PAPER_SIZE_A2      = 2,
    SLOPE_PAPER_SIZE_A3      = 3,
    SLOPE_PAPER_SIZE_A4      = 4,
    SLOPE_PAPER_SIZE_LETTER  = 5,
    SLOPE_PAPER_SIZE_B4      = 6,
    SLOPE_PAPER_SIZE_B5      = 7
}
slope_paper_size_t;


/**
 */
typedef enum _slope_paper_orientation
{
    SLOPE_PAPER_ORIENTATION_PORTRAIT   = 0,
    SLOPE_PAPER_ORIENTATION_LANDSCAPE  = 1
}
slope_paper_orientation_t;


/**
 */
typedef struct _slope_color
{
    double red;
    double green;
    double blue;
    double alpha;
}
slope_color_t;


/**
 * Sets the coordinates of a rectangle
 */
slope_public void
slope_rect_set (slope_rect_t *rect, double x,
                double y, double w, double h);


/**
 */
slope_public void
slope_color_set (slope_color_t *color, double r,
                 double g, double b, double a);


/**
 */
slope_public void
slope_color_set_name (slope_color_t *color,
                      slope_color_name_t name);


/**
 */
slope_public void
slope_cairo_set_color(cairo_t *cr,
                      const slope_color_t *color);


/**
 */
slope_public void
slope_cairo_rectangle(cairo_t *cr,
                      const slope_rect_t *rect);

/**
 */
slope_public void
slope_draw_text(cairo_t *cr, slope_font_t *font,
                double x, double y, const char *text);

/**
 */
slope_public void
slope_get_text_rect(cairo_t *cr, slope_font_t *font,
                    slope_rect_t *rect, const char *text);

SLOPE_END_DECLS

#endif /* SLOPE_PRIMITIVES_H */
