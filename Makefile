########################################################
# C++ compiler
CXX := g++
#CXX := clang++

# C++ compiler flags
# Use the first for debugging, the second for release
CXXFLAGS := -g -Wall -std=c++20 
#CXXFLAGS := -O3 -Wall -std=c++20

#############################################################

TestApp : Driver.cpp ./shared/Maze.so ./shared/MazeBuilder.so ./shared/Wilsons.so ./shared/HuntAndKill.so ./shared/CellularAutomata.so
	$(CXX) $(CXXFLAGS)$^ -o $@

./shared/Maze.so : ./lib/Maze.cpp ./include/Maze.hpp 
	$(CXX) $(CXXFLAGS) -shared $< -fPIC -shared -o $@

./shared/MazeBuilder.so : ./lib/MazeBuilder.cpp ./include/MazeBuilder.hpp 
	$(CXX) $(CXXFLAGS) -shared $< -fPIC -shared -o $@

./shared/Wilsons.so : ./lib/Wilsons.cpp ./include/Wilsons.hpp 
	$(CXX) $(CXXFLAGS) -shared $< -fPIC -shared -o $@

./shared/HuntAndKill.so : ./lib/HK.cpp ./include/HK.hpp  
	$(CXX) $(CXXFLAGS) -shared $< -fPIC -shared -o $@

./shared/CellularAutomata.so : ./lib/CellularAutomata.cpp ./include/CellularAutomata.hpp
	$(CXX) $(CXXFLAGS) -shared $< -fPIC -shared -o $@

#############################################################

clean:
	$(RM) TestApp
	$(RM) ./shared/*.so
