
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

Maze*
buildWilsons (Maze& mz, long long int seed)
{
    Wilsons wil (mz, seed);
    wil.run();
    return &mz;
}

Maze*
buildHuntAndKill (Maze& mz, long long int seed)
{
    HK hunt(mz, seed);
    hunt.run();
    return &mz;
}

Maze*
buildCellularAutomata (Maze& mz, long long int seed)
{
    //TODO: put generate cellular automata call here
    return &mz;
}