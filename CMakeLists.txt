cmake_minimum_required(VERSION 3.15)
project(cbrutev2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_executable(cbrutev2 main.cpp Generator.cpp Generator.h util.h util.cpp)