PROJECT(slope)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(CAIRO "cairo")
PKG_CHECK_MODULES(GTK "gtk+-3.0")

INCLUDE_DIRECTORIES(
    ${CAIRO_INCLUDE_DIRS}
    ${GTK_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
)

SET(SLOPE_HDRS
    slope/list.h
    slope/primitives.h
    slope/scene.h
    slope/metrics.h
    slope/data.h
    slope/xymetrics.h
    slope/xydata.h
)

SET(SLOPE_SRCS
    slope/list.c
    slope/primitives.c
    slope/scene.c
    slope/metrics.c
    slope/data.c
    slope/xymetrics.c
    slope/xydata.c
)

ADD_LIBRARY(slope SHARED ${SLOPE_SRCS})
TARGET_LINK_LIBRARIES(slope ${CAIRO_LIBRARIES})

ADD_EXECUTABLE(app test.c)
TARGET_LINK_LIBRARIES(app slope -lm ${GTK_LIBRARIES})

INSTALL(TARGETS slope DESTINATION /usr/lib)
INSTALL(FILES ${SLOPE_HDRS} DESTINATION /usr/include/slope)
