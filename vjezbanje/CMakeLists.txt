cmake_minimum_required(VERSION 2.8)
project(umjint.vjezbanje)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



add_executable(umjint.vjezbanje.main main.cpp general_search_algorithm.cpp ST_uniform_cost_search.cpp node.cpp map_loader.cpp)# node.cpp map_loader.cpp general_search_algorithm.cpp uniform_cost_search.cpp ST_uniform_cost_search.cpp)
