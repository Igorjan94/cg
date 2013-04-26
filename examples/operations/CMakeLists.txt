cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(operations-example)

add_executable(triangle_contains_point triangle_contains_point.cpp)
target_link_libraries(triangle_contains_point visualization ${GMP_LIBRARIES})

add_executable(contour_contains_point contour_contains_point.cpp)
target_link_libraries(contour_contains_point visualization ${GMP_LIBRARIES})
