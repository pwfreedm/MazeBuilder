
#include <string>
#include <omp.h>
#include <thread>
#include <future>
#include <random>

#include "../include/Maze.hpp"
#include "../include/Wilsons.hpp"
#include "../include/HK.hpp"


Maze parallelize (std::string algo, int length, int width, int seed, int maxCores = std::thread::hardware_concurrency());
std::vector<int>  block_dimensions (int length, int numcores); 
std::vector<Cell> run_algorithm (std::string algorithm, int length, int width, int seed);

Maze
parallelize (std::string algo, int length, int width, int seed, int numcores)
{
    
    //if the requested maze is too small, do it serially
    if (length < numcores * 2 || width < numcores * 2) 
    {
       Maze mz = Maze(length, width); 
       if (algo == "wilsons") { 
        Wilsons(mz, seed);
       }
       else {
        HK(mz, seed);
       }
      return mz; 
    }

    std::minstd_rand0 r(seed);
    std::vector<Cell> mz;
    mz.reserve(length * width);

    std::vector<std::future<std::vector<Cell>>> futures(numcores);
    std::vector<int> blocks = block_dimensions(length, numcores);

    for (int i = 0; i < numcores; ++i)
    {
        futures[i] = std::async(std::launch::async, &run_algorithm, algo, blocks[i], width, r());
    }

    for (auto &f : futures)
    {
        auto mazeFrag = f.get();
        mz.insert(mz.end(), mazeFrag.begin(), mazeFrag.end());
    }
 
    Maze full_mz(mz, width);
    full_mz.openStart();
    full_mz.openEnd();

    return full_mz;
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

std::vector<Cell> 
run_algorithm (std::string algorithm, int length, int width, int seed)
{
    Maze maze(length, width);
    if (algorithm == "wilsons") {
        Wilsons(maze, seed, false);
    }
    else if (algorithm == "hk") {
        HK(maze, seed, false);
    }
    return maze.getMaze(); 
}