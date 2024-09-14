#include "../include/HK.hpp"

    HK::HK (Maze &mz, long long int seed)
    :mz(mz), r(seed) 
    {
        run();
    }

    void
    HK::run()
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

    void
    HK::randomWalk (int startIdx)
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

    int
    HK::validStep(int cur)
    {
        auto neighbors = mz.getNeighbors(cur, false);

        if (neighbors.empty()) { return mz.size(); }

        int move = r() % neighbors.size();
        return mz.getNeighbor(cur, neighbors[move]);
    }

    int
    HK::hunt()
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

inline std::ostream& 
operator<<(std::ostream& os, const HK &hk)
{
    os << hk.mz;
    return os;
}