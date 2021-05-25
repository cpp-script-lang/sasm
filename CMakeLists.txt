cmake_minimum_required(VERSION 3.19)
project(sasm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O")

add_executable(sasm main.cpp)