PROJECT(slope)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#SET(SLOPE_USE_GTK3 TRUE)

FIND_PACKAGE(PkgConfig REQUIRED)

IF(SLOPE_USE_GTK3 MATCHES "TRUE")
    SET(SLOPE_GTK 1)
    PKG_CHECK_MODULES(DEP "gtk+-3.0")
ELSE()
    SET(SLOPE_GTK 0)
    PKG_CHECK_MODULES(DEP "cairo")
ENDIF()

CONFIGURE_FILE (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

INCLUDE_DIRECTORIES(
    ${DEP_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
)

SET(SLOPE_HDRS
    slope/global.h
    slope/primitives.h
    slope/list.h
    slope/figure.h
    slope/metrics.h
    slope/item.h
    slope/xymetrics.h
    slope/xyitem.h
    slope/xyaxis.h
    slope/legend.h
    slope/slope.h
)

IF(SLOPE_USE_GTK3 MATCHES "TRUE")
    SET(SLOPE_HDRS ${SLOPE_HDRS} slope/gtk.h)
ENDIF()

SET(SLOPE_SRCS
    slope/primitives.c
    slope/list.c
    slope/figure.c
    slope/metrics.c
    slope/item.c
    slope/xymetrics.c
    slope/xyitem.c
    slope/xyaxis.c
    slope/legend.c
    slope/slope.c
)

IF(SLOPE_USE_GTK3 MATCHES "TRUE")
    SET(SLOPE_SRCS ${SLOPE_SRCS} slope/gtk.c)
ENDIF()

ADD_LIBRARY(slope SHARED ${SLOPE_SRCS})
TARGET_LINK_LIBRARIES(slope ${DEP_LIBRARIES})

ADD_EXECUTABLE(app test.c)
TARGET_LINK_LIBRARIES(app slope -lm)

INSTALL(TARGETS slope DESTINATION /usr/lib)
INSTALL(FILES ${SLOPE_HDRS} DESTINATION /usr/include/slope)
INSTALL(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION /usr/include/slope)
