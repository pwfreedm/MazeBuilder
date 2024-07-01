########################################################
# C++ compiler
CXX := g++
#CXX := clang++

# C++ compiler flags
# Use the first for debugging, the second for release
CXXFLAGS := -g -Wall -std=c++20 
#CXXFLAGS := -O3 -Wall -std=c++20

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
	$(CXX) $(CXXFLAGS) -c $< -fPIC -shared -o $@\

#############################################################

clean:
	$(RM) TestApp
	$(RM) ./libs/*.so
