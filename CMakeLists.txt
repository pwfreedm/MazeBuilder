cmake_minimum_required(VERSION 3.30.2)
project(Maze)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(Maze src/lib/bindings/Maze_Bindings.cpp)
install(TARGETS Maze DESTINATION .)
