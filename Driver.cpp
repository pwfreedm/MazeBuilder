/** Maze Generation Driver
 * @author Peter Freedman
 * Controls instances of mazes and generates them according to various algorithms.
*/

#include "Maze.hpp"
#include "algorithms/Wilsons.cpp"


int 
main(int argc, char* argv[])
{
    int dimension = std::stoi(argv[1]);
    Wilsons wilson(dimension, dimension);
    std::cout << "through construction\n";
    wilson.run();
    std::cout << "through run\n";
    std::cout << wilson;
}

