/** This file contains helpers that are used across multiple algorithms. */
#pragma once

#include <list>
#include <set>
#include <iostream>
#include <random>
#include "Maze.hpp"

//every method in this file is in the namespace
namespace Tools {
    enum DIRECTION
    {
        UP, DOWN, LEFT, RIGHT
    };

    std::list<unsigned> randomWalk (Maze &mz, unsigned startIdx, bool eraseLoops = true);
    unsigned validStep(const Maze &mz, unsigned prev, unsigned cur);
    void eraseLoop (Maze &mz, std::list<unsigned> &indices, unsigned loopIdx);
}

using namespace Tools;

/*****************************************************************************/
//randomWalk and its helpers

/** Performs a random walk on maze mz. 

    NOTE: this method assumes that at least one cell in the maze has 
    been modified, making that the end of the random walk

    @p mz - the maze to be walked
    @p eraseLoops - a bool deciding whether or not loops are erased

    @return a list of indices representing the walk path
*/
std::list<unsigned>
Tools::randomWalk (Maze &mz, unsigned startIdx, bool eraseLoops)
{
    std::list<unsigned> indices;

    unsigned prev = startIdx;
    unsigned cur = startIdx;
    unsigned temp = startIdx;

    indices.push_back(cur);
    mz[cur].isSeen();

    //if cur's val is 0, cur is not in the maze
    while (mz[cur].val() == 0)
    {
        cur = validStep(mz, temp, cur);
        if (eraseLoops && mz[cur].checkSeen())
        {
            eraseLoop(mz, indices, cur);
            indices.push_back(prev);
        }
        else 
        {
            indices.push_back(cur);
            mz[cur].isSeen();
        }
        temp = prev;
        prev = cur;

    }
    indices.push_back(cur);
    return indices; 
}

/** Returns a valid index in which a random walk can move. 
    
    @p mz - the maze to find the valid index in
    @p cur - the index of the current cell

    @return - an unsigned representing the cur valid index that can be stepped to
    @return - mz.size() if there are no valid directions to move
*/
unsigned
Tools::validStep(const Maze &mz, unsigned prev, unsigned cur)
{
    if(prev > mz.size() || cur > mz.size())
    {
        return mz.size();
    }

    static std::minstd_rand rand(0);

    std::set<int> triedDirs;
    unsigned next = mz.size();

    int nextIDX  = 0;
    while (next >= mz.size() && triedDirs.size() < 4)
    {
        unsigned dir = rand() % 4;
        switch (dir)
        {
            case UP:
                triedDirs.insert(UP);
                nextIDX = cur - mz.width();
                if(nextIDX >= 0 && nextIDX != prev)
                {
                    next = nextIDX;
                }
                break;
            case DOWN: 
                triedDirs.insert(DOWN);
                nextIDX = cur + mz.width();
                if(nextIDX < mz.size() && nextIDX != prev)
                {
                    next = nextIDX;
                }
                break;
            case LEFT:
                triedDirs.insert(LEFT);
                nextIDX = cur % mz.width() - 1;
                if (nextIDX >= 0 && cur - 1 != prev)
                {
                    next = cur - 1;
                }
                break;
            case RIGHT: 
                triedDirs.insert(RIGHT);
                nextIDX = cur % mz.width() + 1;
                if (nextIDX < mz.width() && cur + 1 != prev)
                {
                    next = cur + 1;
                }
                break;
        }
    }
    return next;
}

/** Erases the loop that starts at loopIdx 

    ex: 
    Given the list of indices is A -> B -> C -> D -> E -> F -> G 
    when B is attempted to be readded to the list
    this method will run and the list will instead become A -> B 
    NOTE: the last item before the loop, in this case G, should be readded.
        this logic is left to the randomWalk function.

    @p mz - the maze from which the loop should be erased
    @p indices - the list of indexes from which the loop will be removed
    @p loopIdx - the end of the loop (the index that would reconnect to the existing path)
*/
void
Tools::eraseLoop (Maze &mz, std::list<unsigned> &indices, unsigned loopIdx)
{
    unsigned idx = indices.back();
    while (idx != loopIdx)
    {
        indices.pop_back();
        mz[idx].unSeen();
        idx = indices.back();
    }
}

/*****************************************************************************/
