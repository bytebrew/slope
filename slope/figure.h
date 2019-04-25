#ifndef SLOPE_FIGURE_H
#define SLOPE_FIGURE_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SLOPE_TYPE_FIGURE slope_figure_get_type ()
G_DECLARE_DERIVABLE_TYPE (SlopeFigure, slope_figure, SLOPE, FIGURE, GObject)

struct _SlopeFigureClass
{
  GObjectClass parent_class;

  /* Padding to allow adding up to 12 new virtual
   * functions without breaking ABI. */
  gpointer padding[16];
};


SlopeFigure *slope_figure_new (void);

G_END_DECLS

#endif /* SLOPE_FIGURE_H */
