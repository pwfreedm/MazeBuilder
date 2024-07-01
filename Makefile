########################################################
# Makefile for CSCI 435 
# Authors: Peter Freedman, Dr. Zoppetti
########################################################

########################################################
# Variable definitions
########################################################
# C++ compiler
CXX := g++
#CXX := clang++

# Include directories, prefaced with "-I"
INCDIRS  := 

# C++ compiler flags
# Use the first for debugging, the second for release
CXXFLAGS := -g -Wall -std=c++20 
#CXXFLAGS := -O3 -Wall -std=c++20

# Linker. For C++ should be $(CXX).
LINK := $(CXX)

# Libraries used, prefaced with "-l".
LDLIBS := 

#programs to target with the build
TARGET := Driver
#############################################################
# Rules
#   Rules have the form
#   target : prerequisites
#   	  recipe
#############################################################
TestApp : Driver.cpp ./libs/MazeBuilder.so ./libs/Wilsons.so ./libs/HuntAndKill.so ./libs/CellularAutomata.so
	$(CXX) $(CXXFLAGS)$^ -o $@

./libs/MazeBuilder.so : ./include/MazeBuilder.cpp ./include/Maze.hpp
	$(CXX) $(CXXFLAGS) -c $< -fPIC -shared -o $@

./libs/Wilsons.so : ./algorithms/Wilsons.cpp ./include/Maze.hpp 
	$(CXX) $(CXXFLAGS) -c $< -fPIC -shared -o $@

./libs/HuntAndKill.so : ./algorithms/HK.cpp ./include/Maze.hpp 
	$(CXX) $(CXXFLAGS) -c $< -fPIC -shared -o $@

./libs/CellularAutomata.so : ./algorithms/CellularAutomata.cpp ./include/Maze.hpp 
	$(CXX) $(CXXFLAGS) -c $< -fPIC -shared -o $@
#############################################################
# Clean Section
#	Everything in this block defines the function of 
#	"make clean"
#############################################################
clean:
	$(RM) TestApp
	$(RM) ./libs/*.so
