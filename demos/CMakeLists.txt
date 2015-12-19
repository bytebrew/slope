
add_executable(quick_demo quick.c)
target_link_libraries(quick_demo slope -lm)

add_executable(custom_demo custom.c)
target_link_libraries(custom_demo slope -lm)
