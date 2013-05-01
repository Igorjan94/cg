project(intersections)
cmake_minimum_required(VERSION 2.8)
add_executable(intersections main.cpp)
target_link_libraries(intersections ${GMP_LIBRARIES})
