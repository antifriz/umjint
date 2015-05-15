cmake_minimum_required(VERSION 2.8)
project(umjint.lab2)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -flto -DNDEBUG")

file(GLOB SOURCE_FILES src/*.cpp src/pricier_point_comparator.h src/redundant_comparator.h src/lazy_defines.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})



enable_testing()

add_test(test1 ${PROJECT_NAME} ../data/m1.txt)
add_test(test2 ${PROJECT_NAME} ../data/m2.txt)
add_test(test3 ${PROJECT_NAME} ../data/m3.txt)

add_custom_target(run1 COMMAND ${CMAKE_CTEST_COMMAND} -R test1 --verbose
                  DEPENDS ${PROJECT_NAME})

add_custom_target(run2 COMMAND ${CMAKE_CTEST_COMMAND} -R test2 --verbose
                  DEPENDS ${PROJECT_NAME})

add_custom_target(run3 COMMAND ${CMAKE_CTEST_COMMAND} -R test3 --verbose
                  DEPENDS ${PROJECT_NAME})


add_custom_target(runall DEPENDS run1 run2 run3)
