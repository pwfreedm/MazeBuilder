#include "../include/Wilsons.hpp"

    Wilsons::Wilsons (Maze &mz, long long int seed, bool open_ends)
    :mz(mz), r(seed)
    {
        run(open_ends);
    }

    void
    Wilsons::run(bool open_ends)
    {
        mz.openStart();

        int startIdx = pickStartIdx();
        while (startIdx != mz.size())
        {
            visited.clear();
            randomWalk(startIdx);
            updateMaze(startIdx);
            startIdx = pickStartIdx();
        }
        
        if (!open_ends) { mz[0].left = 0; }
        else { mz.openEnd(); }
    }

    int
    Wilsons::pickStartIdx()
    {
        int setIdx = r() % mz.size();
        for (int i = setIdx; i < mz.size(); ++i)
        {
            if (!mz.inMaze(i)) { return i; }
        }

        for (int i = 0; i < setIdx; ++i)
        {
            if (!mz.inMaze(i)) { return i; }
        }

        return mz.size();
    }

    void
    Wilsons::randomWalk (int startIdx)
    {
        int cur = startIdx; 
        int prev;
        
        while(!mz.inMaze(cur))
        {
            prev = cur; 
            cur = validStep(cur);
            visited[prev] = mz.getDirection(prev, cur);
        }
    }

    int
    Wilsons::validStep(int cur)
    {
        std::vector<Direction> steps;
        steps.reserve(4);

        for(Direction dir : {UP, DOWN, LEFT, RIGHT})
        {
            if (mz.validMove(cur, dir))
            {
                steps.push_back(dir);
            }
        }
        
        int nextDir = r() % steps.size();
        return mz.getNeighbor(cur, steps[nextDir]);
    }

    void 
    Wilsons::updateMaze(int startIdx)
    {
        int curIdx = startIdx; 
        int nextIdx = mz.getNeighbor(startIdx, visited[curIdx]);

        while (visited.contains(mz.getNeighbor(curIdx, visited[curIdx])))
        {
            mz.connect(curIdx, nextIdx);
            curIdx = nextIdx; 
            nextIdx = mz.getNeighbor(curIdx, visited[curIdx]);
        }
        mz.connect(curIdx, nextIdx);
    }

inline std::ostream& 
operator<<(std::ostream& os, const Wilsons &w)
{
    os << w.mz;
    return os;
}
