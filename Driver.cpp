/** Maze Generation Driver
 * @author Peter Freedman
 * Controls instances of mazes and generates them according to various algorithms.
*/

#include "algorithms/Wilsons.cpp"
#include "algorithms/HK.cpp"
#include <string>


int 
main(int argc, char* argv[])
{
    int dimension = std::stoi(argv[1]);
    std::string w("w");
    if (argc == 3)
    {
        std::cout << "Wilsons Algorithm Maze: \n";
        Wilsons wilson(dimension, dimension);
        wilson.run();
        std::cout << wilson;
    } 
    else 
    {
        std::cout << "Hunt and Kill Algorithm Maze: \n";
        HK hk(dimension, dimension);
        hk.run();
        std::cout << hk; 
    }
    
}

