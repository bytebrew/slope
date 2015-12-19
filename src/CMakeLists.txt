
set(slope_HDRS
    slope/global.h
    slope/axis.h
    slope/drawing.h
    slope/figure.h
    slope/item.h
    slope/legend.h
    slope/linear.h
    slope/list.h
    slope/sampler.h
    slope/scale.h
    slope/series.h
)

if(SLOPE_USE_GTK MATCHES "TRUE")
    set(slope_HDRS
        ${slope_HDRS}
        slope/widget.h
    )
endif()

set(slope_SRCS
    slope/axis.c
    slope/drawing.c
    slope/figure.c
    slope/item.c
    slope/legend.c
    slope/linear.c
    slope/list.c
    slope/sampler.c
    slope/scale.c
    slope/series.c
)

if(SLOPE_USE_GTK MATCHES "TRUE")
    set(slope_SRCS
        ${slope_SRCS}
        slope/widget.c
    )
endif()

configure_file(
    slope/config.h.in
    ${CMAKE_BINARY_DIR}/config.h
)

add_library(slope SHARED ${slope_SRCS})
target_link_libraries(slope ${DEP_LIBRARIES})

install(TARGETS slope DESTINATION /usr/lib)
install(FILES ${slope_HDRS} DESTINATION /usr/include/slope)
install(FILES ${CMAKE_BINARY_DIR}/config.h DESTINATION /usr/include/slope)