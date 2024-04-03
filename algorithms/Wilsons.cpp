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
   std::set<int> unvisited;
   std::vector<bool> visited;
   

    public:

        //force passing in a maze
        Wilsons() = delete;

        //maze ctor
        Wilsons (Maze &mz)
        :mz(mz), r(0), unvisited(genUnvisited()), visited(std::vector<bool>(mz.size()))
        {}

        //dimension ctor
        Wilsons (int rowCount, int colCount)
        :mz(Maze(rowCount, colCount)), r(0), unvisited(genUnvisited()), visited(std::vector<bool>(mz.size()))
        {}

        //runs the algorithm on the maze it was constructed with
        void
        run()
        {
            mz.openStart();

            int startIdx;
            while (unvisited.size() > 0)
            {
                startIdx = pickStartIdx();
                std::list<int> walkPath = randomWalk(startIdx);
                updateMaze(walkPath);
            }
            mz.openEnd();
        }

        friend std::ostream& operator<<(std::ostream& os, const Wilsons &w);

   private:
   
    /** Fills unvisited with all valid indices. Skips 0 because it is set 
        during maze generation.
    */
    std::set<int> 
    genUnvisited()
    {
        std::set<int> unvis;
        for(int i = 1; i < mz.size(); ++i)
        {
            unvis.insert(i);
        }
        return unvis;
    }

    /** Picks a random starting index from the set of unvisited indices */
    int
    pickStartIdx()
    {
        int setIdx = r() % unvisited.size();
        return *(std::next(unvisited.begin(), setIdx));
    }

    /** Performs a random walk on maze mz. 

    NOTE: this method assumes that at least one cell in the maze has 
    been modified, making that the end of the random walk

    @p mz - the maze to be walked
    @p eraseLoops - a bool deciding whether or not loops are erased

    @return a list of indices representing the walk path
    */
    std::list<int>
    randomWalk (int startIdx)
    {
        std::list<int> indices;

        int cur = startIdx; 
        int prev;
        
        while(!mz.inMaze(cur))
        {
            indices.push_back(cur);
            visited[cur] = true;
            prev = cur; 
            cur = validStep(cur);
            if (visited[cur] || !mz.hasIndex(cur))
            {
                eraseLoop(indices, prev);
                cur = prev;
            }
        }
        indices.push_back(cur);
        return indices;
    }

    /** Returns a valid index in which a random walk can move. 

    @return - an int representing the cur valid index that can be stepped to
    @return - mz.size() if there are no valid directions to move
    */
    int
    validStep(int cur)
    {
        std::vector<int> neighbors;
        neighbors.reserve(4);

        for (DIRECTION dir : {UP, DOWN, LEFT, RIGHT})
        {
            int nextIdx = mz.getNeighbor(cur, dir);
            if (mz.hasIndex(nextIdx) && !visited[nextIdx])
            {
                neighbors.push_back(nextIdx); 
            }
        }
        if (neighbors.empty()) { return mz.size(); }

        int move = r() % neighbors.size();
        return neighbors[move];
    }

    /** Helper to find a neighbor that has been seen already. 
        NOTE: This method is a helper for beginning loop erasure. 
        It does not search randomly, and so shouldn't be used in 
        any context where a random connected neighbor is needed
    */
    int
    connectedNeighbor(int cur)
    {
        for (DIRECTION dir : {UP, DOWN, LEFT, RIGHT})
        {
            int nextIdx = mz.getNeighbor(cur, dir);
            if (mz.hasIndex(nextIdx) && visited[nextIdx])
            {
                return mz.getNeighbor(cur, dir);
            }
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
    eraseLoop (std::list<int> &indices, int loopIdx)
    {
        int seenNeighbor = connectedNeighbor(loopIdx);

        int idx = indices.back();
        while (idx != seenNeighbor)
        {
            indices.pop_back();
            visited[idx] = false;
            idx = indices.back();
        }
        visited[idx] = false;
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
    updateMaze(std::list<int> walk)
    {
        if(walk.size() < 2) { return; }

        int cur = walk.back();
        int prev;
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
