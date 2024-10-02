
#include <string>
#include <thread>

#include "../include/Maze.hpp"
#include "../include/Wilsons.hpp"
#include "../include/HK.hpp"


Maze parallelize (std::string algo, int length, int width, int seed, int maxCores = std::thread::hardware_concurrency());
std::vector<int>  block_dimensions (int length, int width, int numcores); 

Maze
parallelize (std::string algo, int length, int width, int seed, int numcores)
{
    //if the requested maze is too small, do it serially
    if (length < numcores * 2 || width < numcores * 2) 
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

    std::vector<int> blocks = block_dimensions(length, width, numcores);
    

}

std::vector<int>
block_dimensions (int length, int width, int numcores)
{
    //figuring out the sizes of individual blocks
    int blen, bwid, lenr, widr;
    if ((lenr = length % numcores) == 0) { blen = length / numcores; }
    else { blen = (length - lenr) / numcores; }

    if ((widr = width % numcores) == 0) { bwid = width / numcores; }
    else { bwid = (width - widr) / numcores; }
    //returning length width pairs for each block
    std::vector<int> blocks (numcores * 2);
    for (int i = 0; i < numcores * 2; i += 2)
    {
        if (lenr && i >= (numcores * 2 - lenr)) { blocks[i] = blen + 1; }
        else { blocks[i] = blen; }

        if (widr && i >= (numcores * 2 - widr)) { blocks[i + 1] = bwid + 1; }
        else { blocks[i + 1] = bwid; }
    }
    return blocks;
}