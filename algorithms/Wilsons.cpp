#include <set>
#include <random>
#include <iostream>
#include <list>
#include <vector>

#include "../Maze.hpp"

class Wilsons
{

   Maze mz; 
   std::minstd_rand0 r;
   std::set<unsigned> unvisited;
   std::vector<bool> visited;
   

    public:

        //force passing in a maze
        Wilsons() = delete;

        //maze ctor
        Wilsons (Maze &mz)
        :mz(mz), r(0), unvisited(genUnvisited()), visited(std::vector<bool>(mz.size()))
        {}

        //dimension ctor
        Wilsons (unsigned rowCount, unsigned colCount)
        :mz(Maze(rowCount, colCount)), r(0), unvisited(genUnvisited()), visited(std::vector<bool>(mz.size()))
        {}

        //runs the algorithm on the maze it was constructed with
        void
        run()
        {
            mz.openStart();

            unsigned startIdx;
            while (unvisited.size() > 0)
            {
                startIdx = pickStartIdx();
                std::list<unsigned> walkPath = randomWalk(startIdx);
                updateMaze(walkPath);
            }

            mz.openEnd();
        }

        friend std::ostream& operator<<(std::ostream& os, const Wilsons &w);

   private:
   
    /** Fills unvisited with all valid indices. Skips 0 because it is set 
        during maze generation.
    */
    std::set<unsigned> 
    genUnvisited()
    {
        std::set<unsigned> unvis;
        for(unsigned i = 1; i < mz.size(); ++i)
        {
            unvis.insert(i);
        }
        return unvis;
    }

    /** Picks a random starting index from the set of unvisited indices */
    unsigned
    pickStartIdx()
    {
        unsigned setIdx = r() % unvisited.size();
        return *(std::next(unvisited.begin(), setIdx));
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

        unsigned cur = startIdx; 
        unsigned prev = startIdx;
        
        while(true)
        {
            indices.push_back(cur);
            visited[cur] = true;
            prev = cur; 
            cur = validStep(cur);

            if (mz.hasCell(cur))
            {
                indices.push_back(cur);
                break;
            }
            
            if (visited[cur])
            {
                eraseLoop(indices, prev);
                cur = prev;
            }
        }
        return indices;
    }

    /** Returns a valid index in which a random walk can move. 

    @return - an unsigned representing the cur valid index that can be stepped to
    @return - mz.size() if there are no valid directions to move
    */
    unsigned
    validStep(unsigned cur)
    {
        if(!mz.hasIndex(cur)) { return mz.size(); }

        unsigned next = mz.size();
        while (!mz.hasIndex(next))
        {
            unsigned dir = r() % 4;
            switch (dir)
            {
                case UP:
                    if(mz.validMove(cur, UP))
                    {
                        return mz.getIdx(cur, UP);
                    }
                    break;
                case DOWN: 
                    if(mz.validMove(cur, DOWN))
                    {
                        return mz.getIdx(cur, DOWN);
                    }
                    break;
                case LEFT:
                    if(mz.validMove(cur, LEFT))
                    {
                        return mz.getIdx(cur, LEFT);
                    }
                    break;
                case RIGHT: 
                    if(mz.validMove(cur, RIGHT))
                    {
                        return mz.getIdx(cur, RIGHT);
                    }
                    break;
            }
        }
        std::cout << "Cur idx: " << cur << ", Next idx: " << next << '\n';
        return next;
    }

    /** Helper to find a neighbor that has been seen already. */
    unsigned
    connectedNeighbor(unsigned cur)
    {
        if (mz.validMove(cur, UP) && visited[mz.getIdx(cur, UP)])
        {
            return mz.getIdx(cur, UP);
        }
        if (mz.validMove(cur, DOWN) && visited[mz.getIdx(cur, DOWN)])
        {
            return mz.getIdx(cur, DOWN);
        }
        if (mz.validMove(cur, LEFT) && visited[mz.getIdx(cur, LEFT)])
        {
            return mz.getIdx(cur, LEFT);
        }
        if (mz.validMove(cur, RIGHT) && visited[mz.getIdx(cur, RIGHT)])
        {
            return mz.getIdx(cur, RIGHT);
        }
        return mz.size();
    }

    /** Erases the loop that starts at loopIdx 

    ex: 
    Given the list of indices is A -> B -> C -> D -> E -> F -> G 
    when B is attempted to be readded to the list
    this method will run and the list will instead become A -> B -> G

    @p mz - the maze from which the loop should be erased
    @p indices - the list of indexes from which the loop will be removed
    @p loopIdx - the end of the loop (the index that would reconnect to the existing path)
    */
    void
    eraseLoop (std::list<unsigned> &indices, unsigned loopIdx)
    {
        unsigned seenNeighbor = connectedNeighbor(loopIdx);

        unsigned idx = indices.back();
        while (idx != seenNeighbor)
        {
            indices.pop_back();
            visited[idx] = false;
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
        if(walk.size() < 2) { return; }

        unsigned cur = walk.back();
        unsigned prev;
        do {
            prev = *(std::prev(walk.end(), 2));
            mz.connect(cur, prev);

            //cleanup to mark cur as added
            visited[cur] = false;
            unvisited.erase(cur);

            walk.pop_back();
            cur = prev;

        } while (prev != walk.front());

        //cleanup - this cell was connected already
        visited[prev] = false;
        unvisited.erase(prev);
    }

};

inline std::ostream& 
operator<<(std::ostream& os, const Wilsons &w)
{
    os << w.mz;
    return os;
}
