#pragma once

#include <random>

#include "Maze.hpp"

class HK
{
    friend class MazeBuilder;

    Maze &mz;
    std::minstd_rand r;

protected: 

    //Constructor
    HK (Maze &mz, long long int seed);

    //Runs the algorithm on the provided maze
    void run ();

    //output stream operator. Prints the underlying maze
    friend std::ostream& operator<<(std::ostream& os, const HK &hk);

private: 

    //Performs a random walk on maze mz, starting at startIdx.
    void randomWalk (int startIdx);

    /** Picks a random but valid next index to move to. 

        A valid step is:
        1) a neighbor of cell mz[cur]
        2) within the boundaries of the maze 
        3) not currently connected to the maze
        
        Returns the index of the valid step or mz.size() if there are none
    */
    int validStep (int cur);

    //Hunts for the next cell not currently in the maze that is next to a connected cell.
    int hunt();
};