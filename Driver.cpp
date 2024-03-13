/** Maze Generation Driver
 * @author Peter Freedman
 * Controls instances of mazes and generates them according to various algorithms.
*/

#include "Maze.hpp"
#include "algorithms/Wilsons.cpp"


int 
main(int argc, char* argv[])
{
    Maze m(std::stoi(argv[1]));
    Wilsons wilson(m);
    wilson.run();
    wilson.print();
}

