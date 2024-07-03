
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

void
MazeBuilder::genWilsons (Maze& mz, long long int seed)
{
    lastSeed = seed == -1 ? getSeed() : seed;
    Wilsons wil (mz, lastSeed.value());
    wil.run();
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

void
MazeBuilder::genHuntAndKill (Maze& mz, long long int seed)
{
    lastSeed = seed == -1 ? getSeed() : seed;
    HK hunt(mz, lastSeed.value());
    hunt.run();
}

Maze 
MazeBuilder::genCellularAutomata (int length, int width, long long int seed)
{
    lastSeed = seed == -1 ? getSeed() : seed;
    Maze mz(length, width);
    //TODO: this is where I'd put my call to generate with cellular automata...
    //IF I HAD ONE
    return mz;
}

void
MazeBuilder::genCellularAutomata (Maze& mz, long long int seed)
{
    lastSeed = seed == -1 ? getSeed() : seed;
    //TODO: put generate cellular automata call here
}

extern "C"
{
    MazeBuilder*
    newMazeBuilder ()
    {
        return new MazeBuilder();
    }

    Maze*
    genEmptyMaze (int length, int width)
    {
        return new Maze(length, width);
    }

    long long int
    getLastSeed (MazeBuilder& self)
    {
        return self.getLastSeed().value_or(-1);
    }

    void
    genWilsons (MazeBuilder& self, Maze& mz, long long int seed)
    {
        self.genWilsons(mz, seed);
    }

    void
    genHuntAndKill (MazeBuilder& self, Maze& mz, long long int seed)
    {
        self.genHuntAndKill(mz, seed);
    }

    void
    genCellularAutomata (MazeBuilder& self, Maze& mz, long long int seed)
    {
        self.genCellularAutomata(mz, seed);
    }

}