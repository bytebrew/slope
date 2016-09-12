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
    src/slope/scene.h \
    src/slope/slope.h \
    src/slope/view.h \
    src/slope/scene_p.h

SOURCES += \
    test.c \
    src/slope/drawing.c \
    src/slope/scene.c \
    src/slope/view.c
