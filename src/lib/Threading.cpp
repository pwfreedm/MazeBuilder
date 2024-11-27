
#include <memory>
#include <string>
#include <thread>
#include <span>
#include <random>

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
        mz = Wilsons(mz, seed).release();
       }
       else {
        mz = HK(mz, seed).release();
       }
      return mz; 
    }

    std::minstd_rand0 r(seed);
    //enforce width multiple of 64, pass spans. Cuts allocation in half
    std::vector<int> blocks = block_dimensions(length, numcores);
    
    auto mz = std::make_unique_for_overwrite<Cell[]>( (length * width) / 2);
    std::span maze{mz.get(), static_cast<size_t>(length) * width / 2};

    //scope forces all jthreads to rejoin before smoothing etc
    {
        std::vector<jthread> threads (numcores);
        for (int th = 0; th < numcores; ++th)
        {
            //const values mean each thread gets its own copy 
            int const curr_len = blocks[th];
            size_t const count = width * curr_len;
            std::span curr = maze.subspan(0, count);
            maze = maze.subspan(count);
            threads.emplace_back(run_algorithm, algo, curr, curr_len, width, r());
        }
    }
    
    std::shared_ptr<Cell[]> sh (std::move(mz));
    Maze<Mazeable> full_mz(sh, length, width);
    smooth_edges(full_mz, blocks, seed);
    full_mz.openStart();
    full_mz.openEnd();

    return full_mz;
}

std::vector<int>
block_dimensions (int length, int numcores)
{
    std::vector<int> blocks;
    blocks.reserve(numcores);
    int curr = 0;
    for (int i = 0; i < numcores; ++i) {
        int next = static_cast<size_t>(length) * (i + 1) / numcores;
        blocks.emplace_back(next - curr);
        curr = next;
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
            Side prev_side = mz.getSide(idx - 1);

            if (!prev.up(prev_side) && (r() % 11) > anti_consecutive_bias) 
            { 
                mz.connect(idx, mz.getNeighbor(idx, UP)); 
                anti_consecutive_bias += 3;
            }
            else if (anti_consecutive_bias > 0) { --anti_consecutive_bias; }
        }
    }
}
