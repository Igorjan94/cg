cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(algorithms)

add_executable(douglasPeucker douglasPeucker.cpp)
target_link_libraries(douglasPeucker visualization ${GMP_LIBRARIES})
