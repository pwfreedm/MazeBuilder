########################################################
# C++ compiler
CXX := g++
#CXX := clang++

# C++ compiler flags
# Use the first for debugging, the second for release
CXXFLAGS := -g -Wall -std=c++20 -I/usr/include/python3.12 -lpython3.12
#CXXFLAGS := -O3 -Wall -std=c++20

#############################################################
all: ./shared/Maze.so ./shared/MazeBuilding.so 

Driver: Driver.cpp ./include/Maze.hpp ./include/Wilsons.hpp
	$(CXX) $(CXXFLAGS) Driver.cpp ./lib/Maze.cpp ./lib/Wilsons.cpp -o $@

./shared/Maze.so : ./lib/Maze.cpp ./include/Maze.hpp 
	$(CXX) $(CXXFLAGS) -shared $< -fPIC -o $@

./shared/MazeBuilding.so : ./include/Maze.hpp ./include/Wilsons.hpp ./include/HK.hpp ./include/CellularAutomata.hpp ./lib/Maze.cpp ./lib/Wilsons.cpp ./lib/HK.cpp ./lib/CellularAutomata.cpp ./lib/MazeBuilding.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC ./lib/Maze.cpp ./lib/Wilsons.cpp ./lib/HK.cpp ./lib/CellularAutomata.cpp ./lib/MazeBuilding.cpp -o $@

#############################################################

clean:
	$(RM) Driver
	$(RM) ./shared/*.so

