#include <random>
#include <set>

#include "../Maze.hpp"


class HK
{
    //TODO: Replace this with a call that gets time after testing is done
    const static int RNG_SEED = 0;
    Maze mz;
    int lastIdx;
    std::minstd_rand r;

    public: 

        //ctor
        HK (Maze &mz)
        :mz(mz), lastIdx(0), r(RNG_SEED) {}

        HK (int rowCount, int colCount)
        :mz(rowCount, colCount), lastIdx(0), r(RNG_SEED) {}

        //Runs the algorithm on the provided maze
        void
        run()
        {
            mz.openStart();
            int startIdx = r() % mz.size();
            while(lastIdx != mz.size() - 1)
            {
                randomWalk(startIdx);
                startIdx = hunt();
            }
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

        while (!mz.hasCell(cur))
        {
            prev = cur;
            cur = validStep(cur, prev);
            mz.connect(cur, prev);
        }

    }

    int
    validStep(int cur, int prev)
    {
        if(!mz.hasIndex(cur) || !mz.hasIndex(prev)) { return mz.size(); }

        std::set <DIRECTION> visitedDirs;
        int next = mz.size();
        int nextIdx;
        while (!mz.hasIndex(next) && visitedDirs.size() < 4)
        {
            int dir = r() % 4;
            switch (dir)
            {
                case UP:
                
                    visitedDirs.emplace(UP);
                    nextIdx = mz.getIdx(cur, UP);
                    if(mz.validMove(cur, UP) && nextIdx != prev)
                    {
                        return nextIdx;
                    }
                    break;
                case DOWN: 
                    visitedDirs.emplace(DOWN);
                    nextIdx = mz.getIdx(cur, DOWN);
                    if(mz.validMove(cur, DOWN) && nextIdx != prev)
                    {
                        return nextIdx;
                    }
                    break;
                case LEFT:
                    visitedDirs.emplace(LEFT);
                    nextIdx = mz.getIdx(cur, LEFT);
                    if(mz.validMove(cur, LEFT) && nextIdx != prev)
                    {
                        return nextIdx;
                    }
                    break;
                case RIGHT: 
                    visitedDirs.emplace(RIGHT);
                    nextIdx = mz.getIdx(cur, RIGHT);
                    if(mz.validMove(cur, RIGHT) && nextIdx != prev)
                    {
                        return nextIdx;
                    }
                    break;
            }
        }
        return next;
    }

    /** Hunts for the next cell not currently in the maze that is next to a connected cell. 
        Updates lastIdx as it goes */
    int
    hunt()
    {
        lastIdx = 0;
        while (mz[lastIdx].val() != 0)
        {
            int neighbor = connectedNeighbor(lastIdx);
            if (mz.hasCell(neighbor))
            {
                mz.connect(lastIdx, neighbor);
                return lastIdx;
            }
            ++lastIdx;
        }
        return mz.size();
    }

    /** Helper to find a neighbor that has been seen already. */
    int
    connectedNeighbor(int cur)
    {
        if (mz.validMove(cur, UP) && mz.hasCell(mz.getIdx(cur, UP)))
        {
            return mz.getIdx(cur, UP);
        }
        if (mz.validMove(cur, DOWN) && mz.hasCell(mz.getIdx(cur, DOWN)))
        {
            return mz.getIdx(cur, DOWN);
        }
        if (mz.validMove(cur, LEFT) && mz.hasCell(mz.getIdx(cur, LEFT)))
        {
            return mz.getIdx(cur, LEFT);
        }
        if (mz.validMove(cur, RIGHT) && mz.hasCell(mz.getIdx(cur, RIGHT)))
        {
            return mz.getIdx(cur, RIGHT);
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