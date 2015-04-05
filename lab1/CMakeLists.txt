cmake_minimum_required(VERSION 2.8)
project(ooup)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DPARAM_heur2")


file(GLOB SOURCES src/*.cpp)

add_executable(umjint.runme ${SOURCES})


