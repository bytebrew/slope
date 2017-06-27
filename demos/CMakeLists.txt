include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_BINARY_DIR}"
    "${GTK_INCLUDE_DIRS}"
)

add_executable(simple simple.c)
target_link_libraries(simple slope ${GTK_LIBRARIES} -lm)

add_executable(plot_stack plot_stack.c)
target_link_libraries(plot_stack slope ${GTK_LIBRARIES} -lm)

add_executable(widgets widgets.c)
target_link_libraries(widgets slope ${GTK_LIBRARIES} -lm)

add_executable(animation animation.c)
target_link_libraries(animation slope ${GTK_LIBRARIES} -lm)

