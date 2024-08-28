#CMake Version Control and Top Level Project
cmake_minimum_required(VERSION 3.30.2)
project(Maze)

#CMake Config Options
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_COMPILE Clang++)

#C++ Version Control
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ version selection")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Finding Dependencies
find_package(Python 3.12 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(Maze src/lib/bindings/Maze_Bindings.cpp)
install(TARGETS Maze DESTINATION .)
