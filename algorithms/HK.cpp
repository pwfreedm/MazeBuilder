
#include "../Maze.hpp"


class HK
{
    Maze mz;
    unsigned lastIdx;

    public: 

        //ctor
        HK (Maze &mz)
        :mz(mz), lastIdx(0) {}

        void
        run()
        {

        }
        
        friend std::ostream& operator<<(std::ostream& os, const HK &hk);

};

inline std::ostream& 
    operator<<(std::ostream& os, const HK &hk)
    {
        os << hk.mz;
        return os;
    }