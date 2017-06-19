
file(GLOB_RECURSE slope_SRCS "./slope/*.c")
file(GLOB_RECURSE slope_HDRS "./slope/*.h")

configure_file(
   slope/config.h.in
   ${CMAKE_BINARY_DIR}/config.h
)

add_library(slope SHARED ${slope_SRCS}  ${slope_HDRS})

install(TARGETS slope DESTINATION /usr/lib)
install(FILES ${slope_HDRS} DESTINATION /usr/include/slope)
install(FILES ${CMAKE_BINARY_DIR}/config.h DESTINATION /usr/include/slope)

