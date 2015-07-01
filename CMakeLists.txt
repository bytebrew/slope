
project(slope C)
cmake_minimum_required(VERSION 2.8)


set(SLOPE_USE_GTK3 "TRUE")
find_package(PkgConfig REQUIRED)
if(SLOPE_USE_GTK3 MATCHES "TRUE")
    set(SLOPE_HAVE_GTK 1)
    set(SLOPE_HAVE_PANGO 0) # pango support is still experimental
    PKG_CHECK_MODULES(DEP "gtk+-3.0")
else()
    set(SLOPE_HAVE_GTK 0)
    set(SLOPE_HAVE_PANGO 0)
    PKG_CHECK_MODULES(DEP "cairo")
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/slope-config.h.in"
    "${PROJECT_BINARY_DIR}/slope-config.h"
)


include_directories(
    ${DEP_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
)

set(SLOPE_HDRS
    slope/global.h
    slope/primitives.h
    slope/text.h
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

if(SLOPE_USE_GTK3 MATCHES "TRUE")
    set(SLOPE_HDRS
        ${SLOPE_HDRS}
        slope/view.h
    )
endif()

set(SLOPE_SRCS
    slope/primitives.c
    slope/text.c
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

if(SLOPE_USE_GTK3 MATCHES "TRUE")
    set(SLOPE_SRCS
        ${SLOPE_SRCS}
        slope/view.c
    )
endif()

add_library(slope SHARED ${SLOPE_SRCS})
target_link_libraries(slope ${DEP_LIBRARIES})

add_executable(demo demo.c)
target_link_libraries(demo slope -lm)

install(TARGETS slope DESTINATION /usr/lib)
install(FILES ${SLOPE_HDRS} DESTINATION /usr/include/slope)
install(FILES "${PROJECT_BINARY_DIR}/slope-config.h" DESTINATION /usr/include/slope)
