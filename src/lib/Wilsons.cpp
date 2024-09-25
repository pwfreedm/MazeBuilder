#include "../include/Wilsons.hpp"

    Wilsons::Wilsons (Maze &mz, long long int seed)
    :mz(mz), r(seed), unvisited(genUnvisited()), visited(std::vector<Direction>(mz.size()))
    {
        run();
    }

    void
    Wilsons::run()
    {
        

        int startIdx;
        while (unvisited.size() > 0)
        {
            startIdx = pickStartIdx();
            std::list<int> walkPath = randomWalk(startIdx);
            updateMaze(walkPath);
        }

        mz.openStart();
        mz.openEnd();
    }

    std::set<int> 
    Wilsons::genUnvisited()
    {
        std::set<int> unvis;
        for(int i = 0; i < mz.size(); ++i)
        {
            unvis.insert(i);
        }
        return unvis;
    }

    int
    Wilsons::pickStartIdx()
    {
        int setIdx = r() % unvisited.size();
        return *(std::next(unvisited.begin(), setIdx));
    }

    std::list<int>
    Wilsons::randomWalk (int startIdx)
    {
        std::list<int> indices;

        int cur = startIdx; 
        int prev;
        
        while(!mz.inMaze(cur))
        {
            indices.push_back(cur);
            prev = cur; 
            cur = validStep(cur);
            visited[prev] = mz.getDirection(prev, cur);
        }
        indices.push_back(cur);
        return indices;
    }

    int
    Wilsons::validStep(int cur)
    {
        std::vector<int> neighbors;
        neighbors.reserve(4);

        for (Direction dir : {UP, DOWN, LEFT, RIGHT})
        {
            int nextIdx = mz.getNeighbor(cur, dir);
            if (mz.hasIndex(nextIdx) && !visited[nextIdx])
            {
                neighbors[dir] = nextIdx; 
            }
        }
        if (neighbors.empty()) { return mz.size(); }

        int move = r() % neighbors.size();
        return neighbors[move];
    }

    int
    Wilsons::connectedNeighbor(int cur)
    {
        for (Direction dir : {UP, DOWN, LEFT, RIGHT})
        {
            int nextIdx = mz.getNeighbor(cur, dir);
            if (mz.hasIndex(nextIdx) && visited[nextIdx])
            {
                return mz.getNeighbor(cur, dir);
            }
        }
        return mz.size();
    }

    void
    Wilsons::eraseLoop (std::list<int> &indices, int loopIdx)
    {
        int seenNeighbor = connectedNeighbor(loopIdx);

        int idx = indices.back();
        while (idx > 0 && idx != seenNeighbor)
        {
            indices.pop_back();
            visited[idx] = false;
            idx = indices.back();
        }
        visited[idx] = false;
    }

    void 
    Wilsons::updateMaze(std::list<int> walk)
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

inline std::ostream& 
operator<<(std::ostream& os, const Wilsons &w)
{
    os << w.mz;
    return os;
}
