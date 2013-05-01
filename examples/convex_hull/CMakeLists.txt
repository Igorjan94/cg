cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(convex-hull-example)

add_executable(graham_hull graham_hull.cpp)
target_link_libraries(graham_hull visualization ${GMP_LIBRARIES})

add_executable(andrews_hull andrews.cpp)
target_link_libraries(andrews_hull visualization ${GMP_LIBRARIES})
