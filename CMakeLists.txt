cmake_minimum_required(VERSION 3.17)
project(ergasia2)

set(CMAKE_CXX_STANDARD 14)

add_executable(ergasia2 main.cpp Edge.cpp Edge.h Dijkstra.cpp Dijkstra.h UpdateSSSP.cpp UpdateSSSP.h)