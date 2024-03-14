#include <random>
#include <set>

#include "../Maze.hpp"


class HK
{
    const static int RNG_SEED = 0;
    Maze mz;
    unsigned lastIdx;
    std::minstd_rand rand;

    public: 

        //ctor
        HK (Maze &mz)
        :mz(mz), lastIdx(0), rand(RNG_SEED) {}

        //Runs the algorithm on the provided maze
        void
        run()
        {
            while(lastIdx != mz.size() - 1)
            {
                randomWalk(lastIdx);
                hunt();
            }
        }
        
        friend std::ostream& operator<<(std::ostream& os, const HK &hk);

    private:
         enum
         DIRECTION { UP, DOWN, LEFT, RIGHT };

    /** Performs a random walk on maze mz. 

    NOTE: this method assumes that at least one cell in the maze has 
    been modified, making that the end of the random walk

    @p mz - the maze to be walked
    @p eraseLoops - a bool deciding whether or not loops are erased

    @return a list of indices representing the walk path
    */
    void
    randomWalk (unsigned startIdx)
    {
        std::minstd_rand rand(RNG_SEED);

        unsigned curIdx = startIdx;
        std::set<unsigned> invalidDirs;
        int nextIdx;
        unsigned candidate;

        while (invalidDirs.size() < 4)
        {
            candidate = rand() % 4;

            //if the candidate direction has been checked, don't check again
            if (invalidDirs.contains(candidate)) { continue; }

            switch(candidate)
            {
                case UP: 
                    invalidDirs.insert(UP);
                    nextIdx = curIdx - mz.width();
                    if(nextIdx >= 0 && mz[nextIdx].val() == 0)
                    {
                        mz.connect(curIdx, nextIdx);
                        curIdx = nextIdx;
                    }
                    break; 
                case DOWN: 
                    invalidDirs.insert(DOWN);
                    nextIdx = curIdx + mz.width();
                    if(nextIdx < mz.size() && mz[nextIdx].val() == 0)
                    {
                        mz.connect(curIdx, nextIdx);
                        curIdx = nextIdx;
                    }
                    break;
                case LEFT:
                    invalidDirs.insert(LEFT);
                    nextIdx = curIdx % mz.width() - 1;
                    if (nextIdx >= 0 && mz[nextIdx].val() == 0)
                    {
                       mz.connect(curIdx, nextIdx);
                       curIdx = nextIdx;
                    }
                    break;
                case RIGHT: 
                    invalidDirs.insert(RIGHT);
                    nextIdx = curIdx % mz.width() + 1;
                    if (nextIdx < mz.width() && mz[nextIdx].val() == 0)
                    {
                       mz.connect(curIdx, nextIdx);
                       curIdx = nextIdx;
                    }
                    break;
            }

            //if they are equal, a new cell was connected to the maze
            if(curIdx == nextIdx) { invalidDirs.clear(); }
        }
    }

    /** Hunts for the next cell not currently in the maze. Updates lastIdx as it goes */
    void
    hunt()
    {
        while (mz[lastIdx].val() != 0)
        {
            ++lastIdx;
        }
    }
};




inline std::ostream& 
operator<<(std::ostream& os, const HK &hk)
{
    os << hk.mz;
    return os;
}