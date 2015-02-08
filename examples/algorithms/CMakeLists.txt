cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(algorithms)

#add_executable(douglasPeucker douglasPeucker.cpp)
#target_link_libraries(douglasPeucker visualization ${GMP_LIBRARIES})

add_executable(visibilityGraph visibilityGraph.cpp)
target_link_libraries(visibilityGraph visualization ${GMP_LIBRARIES})

#add_executable(shortestPath shortestPath.cpp)
#target_link_libraries(shortestPath visualization ${GMP_LIBRARIES})

#add_executable(deloneTriangulation deloneTriangulation.cpp)
#target_link_libraries(deloneTriangulation visualization ${GMP_LIBRARIES})

#add_executable(minkowskiSum minkowskiSum.cpp)
#target_link_libraries(minkowskiSum visualization ${GMP_LIBRARIES})

#add_executable(triangleIntersection triangleIntersection.cpp)
#target_link_libraries(triangleIntersection visualization ${GMP_LIBRARIES})

#add_executable(fourTangent fourTangent.cpp)
#target_link_libraries(fourTangent visualization ${GMP_LIBRARIES})

#add_executable(sphere sphere.cpp)
#target_link_libraries(sphere visualization ${GMP_LIBRARIES})

#add_executable(distanceTwoPoligons distanceTwoPoligons.cpp)
#target_link_libraries(distanceTwoPoligons visualization ${GMP_LIBRARIES})

#add_executable(persistent_list persistent_list.cpp)
#target_link_libraries(persistent_list)
