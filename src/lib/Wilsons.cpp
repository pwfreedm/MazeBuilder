#include "../include/Wilsons.hpp"

    Wilsons::Wilsons (Maze &mz, long long int seed)
    :mz(mz), r(seed)
    {
        run();
    }

    void
    Wilsons::run()
    {
        int startIdx = 0;
        while (!mz.inMaze(startIdx))
        {
            visited = std::vector<Direction>(mz.size(), NONE);
            startIdx = pickStartIdx();
            randomWalk(startIdx);
            updateMaze(startIdx);
        }

        mz.openStart();
        mz.openEnd();
    }

    int
    Wilsons::pickStartIdx()
    {
        int setIdx = r() % mz.size();
        while (mz.inMaze(setIdx)) { ++setIdx; }
        return setIdx;
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
        //from startIdx get the index in direction listed in visited
        //
        int curIdx = startIdx; 
        int nextIdx = mz.getNeighbor(startIdx, visited[curIdx]);

        while (!mz.inMaze(curIdx))
        {
            mz.connect(curIdx, nextIdx);
            curIdx = nextIdx; 
            nextIdx = mz.getNeighbor(curIdx, visited[curIdx]);
        }
    }

inline std::ostream& 
operator<<(std::ostream& os, const Wilsons &w)
{
    os << w.mz;
    return os;
}
