#include <random>
#include <vector>

#include "../Maze.hpp"


class CellularAutomata
{
    friend class MazeBuilder;

    Maze& mz;
    std::minstd_rand r;

    CellularAutomata(Maze& mz, long long int seed)
    : mz(mz), r(seed) {}

};