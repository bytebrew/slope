
add_executable(scatter scatter.c)
target_link_libraries(scatter slope -lm)

add_executable(cos cos.c)
target_link_libraries(cos slope -lm)
