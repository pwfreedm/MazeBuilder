#include <set>
#include <random>
#include <iostream>

#include "Maze.hpp"


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
                updateMaze(randomWalk(startIdx));
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

   enum
   DIRECTIONS { UP, DOWN, LEFT, RIGHT};

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

    /** Performs a random walk on maze mz. 

    NOTE: this method assumes that at least one cell in the maze has 
    been modified, making that the end of the random walk

    @p mz - the maze to be walked
    @p eraseLoops - a bool deciding whether or not loops are erased

    @return a list of indices representing the walk path
    */
    std::list<unsigned>
    randomWalk (unsigned startIdx)
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
            cur = validStep(temp, cur);
            if (mz[cur].checkSeen())
            {
                eraseLoop(indices, cur);
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
    validStep(unsigned prev, unsigned cur)
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
    eraseLoop (std::list<unsigned> &indices, unsigned loopIdx)
    {
        unsigned idx = indices.back();
        while (idx != loopIdx)
        {
            indices.pop_back();
            mz[idx].unSeen();
            idx = indices.back();
        }
    }

    /** Updates the maze to reflect the completed walk.
     
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
            if (walk.size() < 2) { break; }

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
