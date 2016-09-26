
add_executable(complete  complete.c)
target_link_libraries(complete slope ${DEPENDENCY_LIBRARIES} -lm)

add_executable(molecules molecules.c)
target_link_libraries(molecules slope ${DEPENDENCY_LIBRARIES} -lm)
