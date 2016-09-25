
add_executable(complete  complete.c)
target_link_libraries(complete slope ${DEPENCENCY_LIBRARIES} -lm)

add_executable(molecules molecules.c)
target_link_libraries(molecules slope ${DEPENCENCY_LIBRARIES} -lm)
