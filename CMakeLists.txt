PROJECT(slope)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(CAIRO "cairo")
INCLUDE_DIRECTORIES(${CAIRO_INCLUDE_DIRS})

SET(SLOPE_HDRS
    slope-list.h
    slope-primitives.h
    slope-scene.h
    slope-metrics.h
    slope-cartesian.h
    slope-xyplot.h
    slope-xyaxis.h
    slope-xychart.h
    slope.h
)

SET(SLOPE_SRCS
    slope-list.c
    slope-primitives.c
    slope-scene.c
    slope-metrics.c
    slope-cartesian.c
    slope-xyplot.c
    slope-xyaxis.c
    slope-xychart.c
)

ADD_LIBRARY(slope SHARED ${SLOPE_SRCS})
TARGET_LINK_LIBRARIES(slope ${CAIRO_LIBRARIES} -lm)

ADD_EXECUTABLE(app test.c)
TARGET_LINK_LIBRARIES(app slope)

INSTALL(TARGETS slope DESTINATION /usr/lib)
INSTALL(FILES ${SLOPE_HDRS} DESTINATION /usr/include/slope)
