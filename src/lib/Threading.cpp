
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
template<CanMaze Mazeable> void smooth_edges (Maze<Mazeable> mz, std::vector<int> blocks, int seed);

template<CanMaze Mazeable = std::unique_ptr<Cell[]>>
Maze<Mazeable>
parallelize (std::string algo, int length, int width, int seed, int numcores)
{
    
    //if the requested maze is too small, do it serially
    if (length < numcores * 4) 
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
    
    size_t internal_maze_size = static_cast<size_t>(length) * width / 2;
    auto mz = std::make_unique_for_overwrite<Cell[]>( internal_maze_size);
    std::span maze{mz.get(), internal_maze_size};

    //scope forces all jthreads to rejoin before smoothing etc
    {
        std::vector<jthread> threads;
        threads.reserve(numcores);

        for (int th = 0; th < numcores; ++th)
        {
            //const values mean each thread gets its own copy 
            int const curr_len = blocks[th];
            size_t const count = (width / 2) * curr_len;
            std::span curr = maze.subspan(0, count);
            maze = maze.subspan(count);
            threads.emplace_back(run_algorithm, algo, curr, curr_len, width, r());
        }
    }
    
    Maze<Mazeable> full_mz(std::move(mz), length, width);
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
smooth_edges (Maze<Mazeable> mz, std::vector<int> blocks, int seed)
{
    std::minstd_rand0 r(seed);

    size_t rownum = 0;
    int anti_consecutive_bias = 0;
    for (auto block : blocks)
    {
        rownum += block; 
        size_t start_idx = rownum * mz.width();
        for (size_t idx = start_idx; idx < start_idx + mz.width(); ++idx)
        {
            Cell prev_cell = mz[idx - 1];
            auto prev_side = mz.getSide(idx - 1);

            //if the previous cell's top face isnt open and a random check is passed
            if(prev_cell.up(prev_side) == 0 && static_cast<int>(r()) % 11 > anti_consecutive_bias)
            {
                mz.connect(idx, mz.getNeighbor(idx, UP));
                anti_consecutive_bias += 3;
            }
            else { --anti_consecutive_bias; }
        } 
    }
}
