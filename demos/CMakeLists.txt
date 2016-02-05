
add_executable(scatter_demo scatter.c)
target_link_libraries(scatter_demo slope -lm)

add_executable(cos_demo cos.c)
target_link_libraries(cos_demo slope -lm)

if (${SLOPE_BUILD_GSL_DEMOS} MATCHES "TRUE")
   add_executable(regression_demo regression.c)
   target_link_libraries(regression_demo slope -lm -lgsl -lgslcblas)
endif()
