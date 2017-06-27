
file(GLOB_RECURSE slope_SRCS "./slope/*.c")
file(GLOB_RECURSE slope_HDRS "./slope/*.h")

configure_file(
   slope/config.h.in
   ${CMAKE_BINARY_DIR}/config.h
)

include_directories(
   "${GTK_INCLUDE_DIRS}"
   "${CMAKE_CURRENT_SOURCE_DIR}"
   "${CMAKE_BINARY_DIR}"
)

add_library(slope SHARED ${slope_SRCS}  ${slope_HDRS})

install(TARGETS slope DESTINATION /usr/lib)
install(FILES ${slope_HDRS} DESTINATION /usr/include/slope)
install(FILES ${CMAKE_BINARY_DIR}/config.h DESTINATION /usr/include/slope)

