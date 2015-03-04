PROJECT(slope)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(CAIRO "cairo")
INCLUDE_DIRECTORIES(${CAIRO_INCLUDE_DIRS})

SET(SLOPE_HDRS
    slope-list.h
    slope-primitives.h
    slope-scene.h
    slope-plotable.h
    slope-cartesian.h
    slope-scatter.h
    slope-cartesian-axis.h
)

SET(SLOPE_SRCS
    slope-list.c
    slope-primitives.c
    slope-scene.c
    slope-plotable.c
    slope-cartesian.c
    slope-scatter.c
    slope-cartesian-axis.c
)

ADD_LIBRARY(slope SHARED ${SLOPE_SRCS})
TARGET_LINK_LIBRARIES(slope ${CAIRO_LIBRARIES} -lm)

ADD_EXECUTABLE(app test.c)
TARGET_LINK_LIBRARIES(app slope)
