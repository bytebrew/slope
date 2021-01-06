lib_LTLIBRARIES = libslope.la

libslope_la_SOURCES = chart.c \
		      figure.c \
		      item.c \
		      legend.c \
		      scale.c \
		      view.c \
		      xyscale.c \
		      drawing.c \
		      figure_p.h \
		      item_p.h \
		      sampler.c \
		      scale_p.h \
		      xyaxis.c \
		      xyseries.c
libslope_la_CFLAGS = -I${top_srcdir}/slope/include/ \
		     -I${top_srcdir}/slope/source/ \
		     @GTK_CFLAGS@
libslope_la_LDFLAGS = @GTK_LIBS@ -rdynamic -lm
