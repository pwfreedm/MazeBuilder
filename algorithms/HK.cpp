#include <random>
#include <vector> //neighbors

#include "../Maze.hpp"


class HK
{
    //TODO: Replace this with a call that gets time after testing is done
    const static int RNG_SEED = 0;
    Maze mz;
    std::minstd_rand r;

    public: 

        //ctor
        HK (Maze &mz)
        :mz(mz), r(RNG_SEED) {}

        HK (int rowCount, int colCount)
        :mz(rowCount, colCount), r(RNG_SEED) {}

        //Runs the algorithm on the provided maze
        void
        run()
        {
            int startIdx = r() % mz.size();
            while(startIdx < mz.size())
            {
                randomWalk(startIdx);
                startIdx = hunt();
            }
            //open after the maze b/c otherwise they can mess with the walks
            mz.openStart();
            mz.openEnd();
        }
        
        friend std::ostream& operator<<(std::ostream& os, const HK &hk);

    private:

    /** Performs a random walk on maze mz, starting at startIdx. */
    void
    randomWalk (int startIdx)
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
    int
    validStep(int cur)
    {
        auto neighbors = mz.getNeighbors(cur, false);

        if (neighbors.empty()) { return mz.size(); }

        int move = r() % neighbors.size();
        return mz.getNeighbor(cur, neighbors[move]);
    }

    /** Hunts for the next cell not currently in the maze that is next to a connected cell. */
    int
    hunt()
    {
        for (int lastIdx = 0; lastIdx < mz.size(); ++lastIdx)
        {
            if (!mz.inMaze(lastIdx))
            {
                auto neighbors = mz.getNeighbors(lastIdx);
                if (!neighbors.empty())
                {
                    DIRECTION dir = neighbors[r() % neighbors.size()];
                    int neighbor =  mz.getNeighbor(lastIdx, dir);
                    mz.connect(lastIdx, neighbor);
                    return lastIdx;
                }
            }
        }
        return mz.size();
    }
};




inline std::ostream& 
operator<<(std::ostream& os, const HK &hk)
{
    os << hk.mz;
    return os;
}