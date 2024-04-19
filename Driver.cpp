/** Maze Generation Driver
 * @author Peter Freedman
 * Controls instances of mazes and generates them according to various algorithms.
*/


#include <string>
#include "MazeBuilder.cpp"
#include "Maze.hpp"

int 
main(int argc, char* argv[])
{
    int dimension = std::stoi(argv[1]);
    MazeBuilder builder;
    
    std::cout << "Wilsons Algorithm Maze: \n";
    Maze wilson = builder.genWilsons(dimension, dimension, 0);
    std::cout << wilson;

    std::cout << "Hunt and Kill Algorithm Maze: \n";
    Maze hk = builder.genHuntAndKill(dimension, dimension, 0);
    std::cout << hk; 
}

