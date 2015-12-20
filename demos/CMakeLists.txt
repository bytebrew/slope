
add_executable(log_demo log.c)
target_link_libraries(log_demo slope -lm)

add_executable(cos_demo cos.c)
target_link_libraries(cos_demo slope -lm)
