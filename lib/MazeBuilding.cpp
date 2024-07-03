
#include <chrono>

//algorithms being built
#include "../include/Maze.hpp"
#include "../include/Wilsons.hpp"
#include "../include/HK.hpp"
#include "../include/CellularAutomata.hpp"
    

/*********************************************************************/
//Definitions for extern interface

long long int getSeed ()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

long long int
buildWilsons (Maze& mz, long long int seed = -1)
{
    auto lastSeed = seed == -1 ? getSeed() : seed;
    Wilsons wil (mz, lastSeed);
    wil.run();
    return lastSeed;
}

long long int
buildHuntAndKill (Maze& mz, long long int seed = -1)
{
    auto lastSeed = seed == -1 ? getSeed() : seed;
    HK hunt(mz, lastSeed);
    hunt.run();
    return lastSeed;
}

long long int
buildCellularAutomata (Maze& mz, long long int seed = -1)
{
    auto lastSeed = seed == -1 ? getSeed() : seed;
    //TODO: put generate cellular automata call here
    return lastSeed;
}

extern "C"
{

    Maze*
    genEmptyMaze (int length, int width)
    {
        return new Maze(length, width);
    }

    long long int
    genWilsons (Maze& mz, long long int seed)
    {
        return buildWilsons(mz, seed);
    }

    long long int
    genHuntAndKill (Maze& mz, long long int seed)
    {
        return buildHuntAndKill(mz, seed);
    }

    long long int
    genCellularAutomata (Maze& mz, long long int seed)
    {
        return buildCellularAutomata(mz, seed);
    }

}