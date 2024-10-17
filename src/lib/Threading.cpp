
#include <memory>
#include <string>
#include <omp.h>
#include <thread>
#include <span>
#include <random>
#include <format>

#include "../include/Maze.hpp"
#include "../include/Wilsons.hpp"
#include "../include/HK.hpp"

using std::jthread; 

template<CanMaze Mazeable> Maze<Mazeable> parallelize (std::string algo, int length, int width, int seed, int maxCores = std::thread::hardware_concurrency());
std::vector<int>  block_dimensions (int length, int numcores); 
void run_algorithm (std::string algorithm, std::span<Cell> maze, int len, int wid, int seed);
template<CanMaze Mazeable> void smooth_edges (Maze<Mazeable>& mz, std::vector<int> blocks, int seed);

template<CanMaze Mazeable = std::unique_ptr<Cell[]>>
Maze<Mazeable>
parallelize (std::string algo, int length, int width, int seed, int numcores)
{
    
    //if the requested maze is too small, do it serially
    if (length < numcores * 2 || width < numcores * 2) 
    {
       Maze<Mazeable> mz = Maze<Mazeable>(length, width); 
       if (algo == "wilsons") { 
        Wilsons(std::move(mz), seed);
       }
       else {
        HK(std::move(mz), seed);
       }
      return mz; 
    }

    std::minstd_rand0 r(seed);
    //enforce width multiple of 64, pass spans. Cuts allocation in half
    std::vector<int> blocks = block_dimensions(length, numcores);
    
    auto mz = std::make_unique_for_overwrite<Cell[]>( length * width);
    auto start = &(mz.get()[0]);

    for (int th = 0; th < numcores; ++th)
    {
        auto end = start + (width * blocks[th]);
        jthread thread(run_algorithm, algo, std::span<Cell>(start, end), blocks[th], width,  r());
        start = end;
        //this is in preparation for smoothing the maze
        if (th > 0) { blocks[th] += blocks[th - 1]; }
    }

    std::shared_ptr<Cell[]> sh(std::move(mz));
    Maze full_mz(sh, length, width);
    smooth_edges(full_mz, blocks, seed);
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

void
run_algorithm (std::string algorithm, std::span<Cell> maze, int length, int width, int seed)
{
    //initialize the memory because algorithms rely on it being zeroed
    //initialize it here so main doesn't do all the init serially
    for (Cell c : maze) { c = Cell(); }
    Maze<std::span<Cell>> m(maze, length, width);

    if (algorithm == "wilsons") {
        Wilsons(m, seed, false);
    }
    else if (algorithm == "hk") {
        HK(m, seed, false);
    }
}

template<CanMaze Mazeable> 
void 
smooth_edges (Maze<Mazeable>& mz, std::vector<int> blocks, int seed)
{
    std::minstd_rand0 r(seed);
    
    for (size_t i = 0; i < blocks.size() - 1; ++i)
    {
        unsigned long anti_consecutive_bias = 0;
        int low_bound = blocks[i] * mz.width();
        for (int idx = low_bound; idx < low_bound + mz.width(); ++idx)
        {
            Cell prev = mz[idx - 1];
            Side prev_side = mz.get_side(idx - 1);

<<<<<<< HEAD
            if (!prev.up(prev_side) && (r() % 7) > anti_consecutive_bias) 
=======
            if (!prev.up && (r() % 11) > anti_consecutive_bias) 
>>>>>>> 9c44d14 (finishing touches)
            { 
                mz.connect(idx, mz.getNeighbor(idx, UP)); 
                anti_consecutive_bias += 3;
            }
            else if (anti_consecutive_bias > 0) { --anti_consecutive_bias; }
        }
    }
}
