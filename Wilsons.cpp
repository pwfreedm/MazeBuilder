#include <algorithm>
#include <cstddef>
#include <set>
#include <random>
#include <iostream>
#include <format> 

#include "Maze.hpp"
#include "Tools.cpp"

using namespace Tools;

class Wilsons
{
   friend class Maze; 

   Maze mz; 
   std::set<unsigned> unvisited; 
    public:
        //ctor.
        Wilsons (Maze &mz)
        :mz(mz)
        {    
                genUnvisited();
        }

        //runs the algorithm on the maze it was constructed with
        void
        run()
        {
            std::minstd_rand rand(0);
            unsigned setIdx; 
            unsigned startIdx;
            while (unvisited.size() > 0)
            {
                setIdx = rand() % unvisited.size();
                startIdx = *(std::next(unvisited.begin(), setIdx));
                updateMaze(randomWalk(mz, startIdx));
            }
            //update the right side of the last element so there is an exit to the maze
            mz[mz.size() - 1].setRight();
        }

        void
        print()
        {
            std::cout << mz << std::endl;
        }

   private:

    /** Fills unvisited with all valid indices. Skips 0 because it is set 
        during maze generation.
    */
    void genUnvisited()
    {
        for(int i = 1; i < mz.size(); ++i)
        {
            unvisited.insert(i);
        }
    }

    /** Updates the maze to reflect the completed walk. Fails if walk.size() < 2. 
     
         NOTE: The steps to perform this update are as follows:

            cur is the end of the walk
            do {
                prev is the idx before cur
                connect cur and prev in the maze
                mark cur as unseen and erase it from unvisited
                remove it from the walk
                set cur to prev
            } while prev isn't the first element in walk
            mark prev as unseen
    */
    void 
    updateMaze(std::list<unsigned> walk)
    {
        unsigned cur = walk.back();
        unsigned prev;
        do {
            prev = *(std::prev(walk.end(), 2));
            mz.connect(cur, prev);
            mz[cur].unSeen();
            unvisited.erase(cur);
            walk.pop_back();
            cur = prev;
        } while (prev != walk.front());

        mz[prev].unSeen();
    }
};
