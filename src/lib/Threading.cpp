
#include <string>
#include <thread>

#include "../include/Maze.hpp"
#include "../include/Wilsons.hpp"
#include "../include/HK.hpp"

Maze parallelize (std::string algo, int length, int width, int seed, int maxCores = std::thread::hardware_concurrency());
std::vector<int>  block_dimensions (int length, int numcores); 

Maze
parallelize (std::string algo, int length, int width, int seed, int numcores)
{
    //if the requested maze is too small, do it serially
    if (length < numcores * 2 && width < numcores * 2) 
    {
       Maze mz = Maze(length, width); 
       if (algo == "Wilsons") { 
        Wilsons(mz, seed);
       }
       else {
        HK(mz, seed);
       }
      return mz; 
    }

    std::vector<int> blocks = block_dimensions(length, numcores);

    for (int i = 0; i < numcores; ++i)
    {
        //create a maze of size blocks[i], width
        //pass it, algorithm, and seed to a new thread
    }

    //another for loop to wait for all the threads, recollecting as needed. 
    //is ordering really necessary? 

    //another loop to reconnect/smooth. 

    return Maze(1,1);
}

std::vector<int>
block_dimensions (int length, int numcores)
{
    //figuring out the sizes of individual blocks
    int blen, lenr;
    if ((lenr = length % numcores) == 0) { blen = length / numcores; }
    else { blen = (length - lenr) / numcores; }

    //returning length width pairs for each block
    std::vector<int> blocks (numcores);
    for (int i = 0; i < numcores; ++i)
    {
        if (lenr > 0) { 
            blocks[i] = blen + 1; 
            --lenr;
        }
        else { blocks[i] = blen; }
    }
    return blocks;
}