#ifndef SLOPE_FIGURE_H
#define SLOPE_FIGURE_H

#include <cairo/cairo.h>
#include <glib-object.h>
#include <slope/drawing.h>

G_BEGIN_DECLS

#define SLOPE_TYPE_FIGURE slope_figure_get_type ()
G_DECLARE_DERIVABLE_TYPE (SlopeFigure, slope_figure, SLOPE, FIGURE, GObject)


typedef struct {
    cairo_t *cr;
} SlopeDC;


struct _SlopeFigureClass
{
  GObjectClass parent_class;
  void (*draw) (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect);

  /* Padding to allow adding up to 12 new virtual
   * functions without breaking ABI. */
  gpointer padding[16];
};


SlopeFigure *slope_figure_new (void);

void slope_figure_draw (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect);

G_END_DECLS

#endif /* SLOPE_FIGURE_H */
