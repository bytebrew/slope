
set(slope_HDRS
   slope/drawing.h
   slope/figure.h
   slope/global.h
   slope/item.h
   slope/scale.h
   slope/slope.h
   slope/view.h
   slope/xyscale.h
   slope/xyseries.h
   slope/xyaxis.h
   slope/legend.h
   slope/sampler.h
)

if(SLOPE_USE_GTK MATCHES "TRUE")
   set(slope_HDRS
      ${slope_HDRS}
      slope/view.h
      slope/chart.h
   )
endif()
   
set(slope_SRCS
   slope/drawing.c
   slope/figure.c
   slope/item.c
   slope/scale.c
   slope/xyscale.c
   slope/xyseries.c
   slope/xyaxis.c
   slope/legend.c
   slope/sampler.c
)

if(SLOPE_USE_GTK MATCHES "TRUE")
   set(slope_SRCS
      ${slope_SRCS}
      slope/view.c
      slope/chart.c
   )
endif()

configure_file(
   slope/config.h.in
   ${CMAKE_BINARY_DIR}/config.h
)

add_library(slope SHARED ${slope_SRCS}  ${slope_HDRS})

install(TARGETS slope DESTINATION /usr/lib)
install(FILES ${slope_HDRS} DESTINATION /usr/include/slope)
install(FILES ${CMAKE_BINARY_DIR}/config.h DESTINATION /usr/include/slope)
