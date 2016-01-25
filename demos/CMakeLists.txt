
add_executable(scatter_demo scatter.c)
target_link_libraries(scatter_demo slope -lm)

add_executable(cos_demo cos.c)
target_link_libraries(cos_demo slope -lm)

