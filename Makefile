########################################################
# C++ compiler
CXX := g++
#CXX := clang++

# C++ compiler flags
# Use the first for debugging, the second for release
CXXFLAGS := -g -Wall -std=c++20 
#CXXFLAGS := -O3 -Wall -std=c++20

#############################################################
all: ./shared/Maze.so ./shared/MazeBuilder.so 

./shared/Maze.so : ./lib/Maze.cpp ./include/Maze.hpp 
	$(CXX) $(CXXFLAGS) -shared $< -fPIC -o $@

./shared/MazeBuilder.so : ./include/Maze.hpp ./include/Wilsons.hpp ./include/HK.hpp ./include/CellularAutomata.hpp ./include/MazeBuilder.hpp ./lib/Maze.cpp ./lib/Wilsons.cpp ./lib/HK.cpp ./lib/CellularAutomata.cpp ./lib/MazeBuilder.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC ./lib/Maze.cpp ./lib/Wilsons.cpp ./lib/HK.cpp ./lib/CellularAutomata.cpp ./lib/MazeBuilder.cpp -o $@

#############################################################

clean:
	$(RM) TestApp
	$(RM) ./shared/*.so
