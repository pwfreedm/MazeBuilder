/** Maze Generation Controller
 * @author Peter Freedman
 * Controls instances of mazes and generates them according to various algorithms.
*/
#include <iostream>
#include "Maze.hpp"
#include "Wilsons.cpp"


int 
main()
{
    Maze m(6);
    Wilsons wilson(m);
    wilson.run();
    wilson.print();
}

