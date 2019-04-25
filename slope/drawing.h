#ifndef SLOPE_DRAWING_H
#define SLOPE_DRAWING_H

#if !defined (SLOPE_H_INSIDE) && !defined (SLOPE_COMPILATION)
#error "Only <slope.h> can be included directly."
#endif

#include <slope/color.h>
#include <cairo/cairo.h>

G_BEGIN_DECLS

typedef struct {
    double x;
    double y;
} SlopePoint;

typedef struct {
    double x;
    double y;
    double width;
    double height;
} SlopeRect;


void slope_set_color (cairo_t *cr, SlopeRGBA color);

void slope_move_to (cairo_t *cr, const SlopePoint *p);

void slope_line_to (cairo_t *cr, const SlopePoint *p);

void slope_rect (cairo_t *cr, const SlopeRect *r);

void slope_draw (cairo_t *cr, SlopeRGBA stroke, SlopeRGBA fill);

G_END_DECLS

#endif /* SLOPE_DRAWING_H */
