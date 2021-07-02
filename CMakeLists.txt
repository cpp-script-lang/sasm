cmake_minimum_required(VERSION 3.19)
project(sasm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")
include_directories("submodules/tabulate/include" ".")
set(SASM_LIB errors.hpp tools.hpp tokens.hpp options.hpp sasm_interpreter.hpp globals.hpp)

add_library(sasmutils ${SASM_LIB})
set_target_properties(sasmutils PROPERTIES LINKER_LANGUAGE CXX)
add_executable(sasm main.cpp)
target_link_libraries(sasm sasmutils)