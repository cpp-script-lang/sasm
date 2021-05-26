cmake_minimum_required(VERSION 3.19)
project(sasm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O")
include_directories("submodules/tabulate/include")
add_executable(sasm main.cpp)