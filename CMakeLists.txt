#CMake Version Control and Top Level Project
cmake_minimum_required(VERSION 3.29.2)
project($(SKBUILD_PROJECT_NAME) LANGUAGES CXX)

#CMake Config Options
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_DEBUG_TARGET_PROPERTIES on)
set(CMAKE_CXX_COMPILE Clang++)

#C++ Version Control
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ version selection")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Finding Dependencies
find_package(Python 3.12 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

#extra files that will be needed
include_directories(./lib)

#Creating the module 
pybind11_add_module(maze src/lib/Maze_Bindings.cpp WITH_SOABI)
target_compile_options(maze PUBLIC -O3 -Wall -Wextra -Werror)
install(TARGETS maze DESTINATION ./)
