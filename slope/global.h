#ifndef SLOPE_GLOBAL_H
#define SLOPE_GLOBAL_H

#if !defined (SLOPE_H_INSIDE) && !defined (SLOPE_COMPILATION)
#error "Only <slope.h> can be included directly."
#endif

#include <glib.h>

#define slope_set(X,b)           X |= (b)
#define slope_clear(X,b)         X &= ~(b)
#define slope_set_nth(X,n)       X |= ((1UL) << (n))
#define slope_clear_nth(X,n)     X &= ~((1UL) << (n))
#define slope_enabled(X,n)       (((X) & (n)) == (n))
#define slope_enabled_any(X,n)   (((X) & (n)) != 0)

G_BEGIN_DECLS

guint32 slope_version_number();

G_END_DECLS

#endif /* SLOPE_GLOBAL_H */
