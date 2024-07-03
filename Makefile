########################################################
# C++ compiler
CXX := g++
#CXX := clang++

# C++ compiler flags
# Use the first for debugging, the second for release
CXXFLAGS := -g -Wall -std=c++20 
#CXXFLAGS := -O3 -Wall -std=c++20

#############################################################

TestApp : Driver.cpp ./shared/Maze.so ./shared/MazeBuilder.so 
	$(CXX) $(CXXFLAGS)$^ -o $@

./shared/Maze.so : ./lib/Maze.cpp ./include/Maze.hpp 
	$(CXX) $(CXXFLAGS) -shared $< -fPIC -o $@

./shared/MazeBuilder.so : ./lib/MazeBuilder.cpp ./include/MazeBuilder.hpp ./lib/Wilsons.cpp ./include/Wilsons.hpp ./lib/HK.cpp ./include/HK.hpp ./lib/CellularAutomata.cpp ./include/CellularAutomata.hpp
	$(CXX) $(CXXFLAGS) -shared $< -fPIC -o $@

#############################################################

clean:
	$(RM) TestApp
	$(RM) ./shared/*.so
