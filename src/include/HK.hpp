#pragma once

#include <random>

#include "Maze.hpp"

template <CanMaze Mazeable = std::unique_ptr<Cell[]>>
class HK
{
    Maze<Mazeable> &mz;
    std::minstd_rand r;

public: 

    HK () = delete;
    
    //Constructor
    HK (Maze<Mazeable> mz, long long int seed, bool open_ends = true)
    :mz(mz), r(seed)
    {
        run(open_ends);
    }

    //Runs the algorithm on the provided maze
    void run (bool open_ends)
    {
        int startIdx = r() % mz.size();
        while(startIdx < mz.size())
        {
            randomWalk(startIdx);
            startIdx = hunt();
        }
        
        if (open_ends)
        {
            mz.openStart();
            mz.openEnd();
        }
    }

    //output stream operator. Prints the underlying maze
    template<CanMaze MazeType>
    friend std::ostream& operator<<(std::ostream& os, const HK<MazeType> &hk);

private: 

    //Performs a random walk on maze mz, starting at startIdx.
    void randomWalk (int startIdx)
    {
        int prev;
        int cur = startIdx;

        while (cur < mz.size())
        {
            prev = cur;
            cur = validStep(cur);
            mz.connect(cur, prev);
        }
    }

    /** Picks a random but valid next index to move to. 

        A valid step is:
        1) a neighbor of cell mz[cur]
        2) within the boundaries of the maze 
        3) not currently connected to the maze
        
        Returns the index of the valid step or mz.size() if there are none
    */
    int validStep (int cur)
    {
        auto neighbors = mz.getNeighbors(cur, false);

        if (neighbors.empty()) { return mz.size(); }

        int move = r() % neighbors.size();
        return mz.getNeighbor(cur, neighbors[move]);
    }

    //Hunts for the next cell not currently in the maze that is next to a connected cell.
    int hunt()
    {
        for (int lastIdx = 0; lastIdx < mz.size(); ++lastIdx)
        {
            if (!mz.inMaze(lastIdx))
            {
                auto neighbors = mz.getNeighbors(lastIdx);
                if (!neighbors.empty())
                {
                    Direction dir = neighbors[r() % neighbors.size()];
                    int neighbor =  mz.getNeighbor(lastIdx, dir);
                    mz.connect(lastIdx, neighbor);
                    return lastIdx;
                }
            }
        }
        return mz.size();
    }
};

template <CanMaze Mazeable>
inline std::ostream& 
operator<<(std::ostream& os, const HK<Mazeable> &hk)
{
    os << hk.mz;
    return os;
}