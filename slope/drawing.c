
#include <slope/drawing.h>

void slope_set_color (cairo_t *cr, SlopeRGBA color) {
    cairo_set_source_rgba(cr,
              slope_rgba_Rf(color),
              slope_rgba_Gf(color),
              slope_rgba_Bf(color),
              slope_rgba_Af(color));

}


void slope_move_to (cairo_t *cr, const SlopePoint *p) {
    cairo_move_to(cr, p->x, p->y);
}


void slope_line_to (cairo_t *cr, const SlopePoint *p) {
    cairo_line_to(cr, p->x, p->y);
}


void slope_rect (cairo_t *cr, const SlopeRect *r) {
    cairo_rectangle(cr, r->x, r->y, r->width, r->height);
}


void slope_draw (cairo_t *cr, SlopeRGBA stroke, SlopeRGBA fill) {
    if (slope_rgba_visible(stroke) && slope_rgba_visible(fill)) {
        slope_set_color(cr, fill);
        cairo_fill_preserve(cr);
        slope_set_color(cr, stroke);
        cairo_stroke(cr);
    } else if (slope_rgba_visible(stroke)) {
        slope_set_color(cr, stroke);
        cairo_stroke(cr);
    } else if (slope_rgba_visible(fill)) {
        slope_set_color(cr,fill);
        cairo_fill(cr);
    }
}
