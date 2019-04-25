#ifndef SLOPE_VIEW_H
#define SLOPE_VIEW_H

#include <gtk/gtk.h>
#include <slope/figure.h>

G_BEGIN_DECLS

#define SLOPE_TYPE_VIEW slope_view_get_type ()
G_DECLARE_DERIVABLE_TYPE (SlopeView, slope_view, SLOPE, VIEW, GtkDrawingArea)

struct _SlopeViewClass
{
  GtkDrawingAreaClass parent_class;

  /* Padding to allow adding up to 12 new virtual
   * functions without breaking ABI. */
  gpointer padding[16];
};


GtkWidget *slope_view_new (void);

SlopeFigure *slope_view_get_figure (SlopeView *self);

G_END_DECLS

#endif /* SLOPE_VIEW_H */
