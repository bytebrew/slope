bin_PROGRAMS = animation \
	       plot_stack \
	       simple \
	       widgets

AM_CFLAGS = -I${top_srcdir}/slope/include/ \
            -I${top_srcdir}/slope/source/
AM_LDFLAGS = -L$(top_builddir)/slope/source/slope/.libs/ -lslope -lm

animation_SOURCES = animation.c
animation_CFLAGS = $(AM_CFLAGS) @GTK_CFLAGS@
animation_LDFLAGS = $(AM_LDFLAGS) @GTK_LIBS@

plot_stack_SOURCES = plot_stack.c
plot_stack_CFLAGS = $(AM_CFLAGS) @GTK_CFLAGS@
plot_stack_LDFLAGS = $(AM_LDFLAGS) @GTK_LIBS@

simple_SOURCES = simple.c
simple_CFLAGS = $(AM_CFLAGS) @GTK_CFLAGS@
simple_LDFLAGS = $(AM_LDFLAGS) @GTK_LIBS@

widgets_SOURCES = widgets.c
widgets_CFLAGS = $(AM_CFLAGS) @GTK_CFLAGS@
widgets_LDFLAGS = $(AM_LDFLAGS) @GTK_LIBS@
