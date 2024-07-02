
#include <chrono>

#include "../include/MazeBuilder.hpp"

//algorithms being built
#include "../include/Wilsons.hpp"
#include "../include/HK.hpp"
#include "../include/CellularAutomata.hpp"
    
long long int
MazeBuilder::getSeed()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

MazeBuilder::MazeBuilder ()
: lastSeed() {}


std::optional<long long int>
MazeBuilder::getLastSeed ()
{
    return lastSeed;
}

Maze
MazeBuilder::genWilsons (int length, int width, long long int seed)
{
    lastSeed = seed == -1 ? getSeed() : seed;
    Maze mz(length, width);
    Wilsons wil(mz, lastSeed.value());
    wil.run();
    return mz;
}


Maze
MazeBuilder::genHuntAndKill (int length, int width, long long int seed)
{
    lastSeed = seed == -1 ? getSeed() : seed;
    Maze mz(length, width);
    HK hunt(mz, lastSeed.value());
    hunt.run();
    return mz;
}


Maze 
MazeBuilder::genCellularAutomata (int length, int width, long long int seed)
{
    lastSeed = seed == -1 ? getSeed() : seed;
    Maze mz(length, width);
    //TODO: this is where I'd put my call to generate with cellular automata...
    //IF I HAD ONE
    HK hunt (mz, lastSeed.value());
    hunt.run();
    return mz;
}
