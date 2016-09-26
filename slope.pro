TEMPLATE = app
TARGET = slope_test
CONFIG -= qt
INCLUDEPATH += src
QMAKE_CC = gcc
QMAKE_CXX = gcc
CONFIG += link_pkgconfig
PKGCONFIG = gtk+-3.0

HEADERS += \
    src/slope/drawing.h \
    src/slope/global.h \
    src/slope/slope.h \
    src/slope/view.h \
    src/slope/figure.h \
    src/slope/figure_p.h \
    src/slope/scale.h \
    src/slope/scale_p.h \
    src/slope/scale_p.h \
    src/slope/item.h \
    src/slope/item_p.h \
    src/slope/xyscale.h \
    src/slope/xyscale_p.h \
    src/slope/chart.h \
    src/slope/xyseries.h \
    src/slope/xyaxis.h \
    src/slope/xyaxis-sampler.h


SOURCES += \
    demos/plot_stack.c \
    src/slope/drawing.c \
    src/slope/view.c \
    src/slope/figure.c \
    src/slope/scale.c \
    src/slope/item.c \
    src/slope/xyscale.c \
    src/slope/chart.c \
    src/slope/xyseries.c \
    src/slope/xyaxis.c \
    src/slope/xyaxis-sampler.c
