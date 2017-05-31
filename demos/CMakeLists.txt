
add_executable(simple simple.c)
target_link_libraries(simple slope ${DEPENDENCY_LIBRARIES} -lm)

add_executable(plot_stack plot_stack.c)
target_link_libraries(plot_stack slope ${DEPENDENCY_LIBRARIES} -lm)

add_executable(widgets widgets.c)
target_link_libraries(widgets slope ${DEPENDENCY_LIBRARIES} -lm)

add_executable(animation animation.c)
target_link_libraries(animation slope ${DEPENDENCY_LIBRARIES} -lm)
