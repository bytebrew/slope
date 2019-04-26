TEMPLATE = app
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += gtk+-3.0
DEFINES += SLOPE_COMPILATION

SOURCES += \
        main.c \
    slope/global.c \
    slope/view.c \
    slope/figure.c \
    slope/drawing.c

HEADERS += \
    slope/global.h \
    slope/color.h \
    slope/slope.h \
    slope/view.h \
    slope/figure.h \
    slope/drawing.h \
    slope/figure_p.h
