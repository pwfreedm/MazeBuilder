########################################################
# C++ compiler
CXX := g++
#CXX := clang++

# C++ compiler flags
# Use the first for debugging, the second for release
CXXFLAGS := -g -Wall -fdeclspec -std=c++20 
#CXXFLAGS := -O3 -Wall -fdeclspec -std=c++20

#############################################################

TestApp : Driver.cpp ./shared/MazeBuilder.so ./shared/Wilsons.so ./shared/HuntAndKill.so ./shared/CellularAutomata.so
	$(CXX) $(CXXFLAGS)$^ -o $@

./shared/MazeBuilder.so : ./lib/MazeBuilder.cpp ./include/MazeBuilder.hpp 
	$(CXX) $(CXXFLAGS) -c $< -fPIC -shared -o $@

./shared/Wilsons.so : ./lib/Wilsons.cpp ./include/Wilsons.hpp 
	$(CXX) $(CXXFLAGS) -c $< -fPIC -shared -o $@

./shared/HuntAndKill.so : ./lib/HK.cpp ./include/HK.hpp  
	$(CXX) $(CXXFLAGS) -c $< -fPIC -shared -o $@

./shared/CellularAutomata.so : ./lib/CellularAutomata.cpp ./include/CellularAutomata.hpp
	$(CXX) $(CXXFLAGS) -c $< -fPIC -shared -o $@

#############################################################

clean:
	$(RM) TestApp
	$(RM) ./shared/*.so
