/**
 @a - Peter Freedman
 @n - maze.hpp
 This is a header that represents an unweighted, non-directed graph
 in a space efficient manner.
*/

#pragma once

#include <ostream>
#include <type_traits> //is_integral
#include <vector>      //cell storage in maze


/**A cell in the maze consists of 5 bytes, [up, down, left, right, seen]
    By default, all five values are initialized to false. Seen is used
    for tracking whether or not the cell is already included in a random walk
    without modifying its connections (which is the criteria for it being 
    considered a permanent part of the maze)
*/
struct Cell 
{
    bool up{false};
    bool down{false};
    bool left{false};
    bool right{false};


    //Default ctor
    Cell()
    {}

    //Value ctor
    Cell(bool up, bool down, bool left, bool right)
    : up(up), down(down), left(left), right(right){}

    /** Returns an int representation of this node using the following method:
        (up << 3) + (down << 2) + (left << 1) + right
    */
    int 
    val() const
    {
        return (up << 3) + (down << 2) + (left << 1) + right;
    }

    /** Equality op - Checks if they return the same thing from Cell.val() 
            and are both seen
    */
    bool
    operator== (const Cell &o)
    {
        return val() == o.val();
    }

    /** Inequality op - inverts operator== */
    bool 
    operator!= (const Cell &o)
    {
        return !(*this == o);
    }

    /** Compares this cell to another cell

        @p o - the cell to compare to this one

        @return - 0 if their fields are set the same
        @return - <0 if o.val > this.val
        @return - >0 if this.val > o.val
    */
    int
    compare(const Cell &o) const
    {
        return val() - o.val();
    }

    /** Sets direction dir to true (removes that wall from the cell) */
    void
    setDirection (DIRECTION dir)
    {
        switch (dir)
        {
            case DIRECTION::UP:
                up = true; 
                break;
            case DIRECTION::DOWN:
                down = true; 
                break;
            case DIRECTION::LEFT:
                left = true; 
                break;
            case DIRECTION::RIGHT:
                right = true; 
                break;
        }
    }

    /** Flips the state of direction dir */
    void
    updateDirection(DIRECTION dir)
    {
        switch (dir)
        {
            case DIRECTION::UP:
                up = !up; 
                break;
            case DIRECTION::DOWN:
                down = !down; 
                break;
            case DIRECTION::LEFT:
                left = !left; 
                break;
            case DIRECTION::RIGHT:
                right = !right; 
                break;
        }
    }
};

//Direction enum for maze methods
enum class DIRECTION { UP, DOWN, LEFT, RIGHT };

class Maze
{
    std::vector<Cell> maze; 
    unsigned len;
    unsigned wid;
    
    public:

    friend std::ostream& operator<< (std::ostream& os, const Maze& mz);

    //require dimensions
    Maze() = delete;

    /** Value ctor - Maze of length x width size

        @param length - the number of rows of cells in the maze
        @param width - the number of columns of cells in the maze 
    */
    Maze(unsigned length, unsigned width)
    :maze(length * width), len(length), wid(width)
    {}

    /** Value ctor - Maze of edgeLen * edgeLen size
     * 
     * @param edgeLen - the length both edges of the maze should be.
    */
    Maze(unsigned edgeLen)
    :maze(edgeLen * edgeLen), len(edgeLen), wid(edgeLen)
    {}

    /** Equality op - Compares fields from smallest to largest*/
    bool
    operator== (Maze& o)
    {
        return 
               len != o.len ? false :
               wid != o.wid ? false :
               maze.data() != o.maze.data() ? false : true;
    }

    /** Inequality op - negates equality op */
    bool
    operator!= (Maze &o)
    {
        return !(*this == o);
    }

    /** Accessor for length - returns the number of rows of cells in this maze*/
    unsigned
    length()
    { return len; }
   
    /** const Accessor for length - returns the number of rows of cells in this maze*/
    unsigned
    length() const
    { return len; }

    /** Accessor for width - returns the number of cols of cells in this maze*/
    unsigned
    width()
    { return wid; }

    /** const Accessor for width - returns the number of cols of cells in this maze*/
    unsigned
    width() const
    { return wid; }

    /** Acessor for the size of the maze. Multiplies len and wid fields. */
    unsigned
    size()
    {
        return len * wid;
    }

    /** const accessor for the size of the maze. Multiplies len and wid fields */
    unsigned
    size() const
    {
        return len * wid;
    }


    /** Element access operator for a maze. Returns a reference to the element
        at position row * maze width + col
        
        NOTE: No bounds checking is performed

        @param row - the row number of the element 
        @param col - the column number of the element
    */
    Cell&
    get(unsigned row, unsigned col)
    {
        return maze[row * wid + col];
    }

    /** Element access operator for a maze. Returns a reference to the element
        at position row * maze width + col
        
        NOTE: No bounds checking is performed

        @param row - the row number of the element 
        @param col - the column number of the element
    */
    const Cell&
    get(unsigned row, unsigned col) const 
    {
        return maze[row * wid + col];
    }

    /** Puts a cell into the maze at maze[row][col]
    
        @param row - the row of the maze in which to insert @p cell
        @param col - the column of the maze in which to inser @p cell
        @param cell - the cell to be inserted into the maze
    */
    void
    set(unsigned row, unsigned col, const Cell& cell)
    {
        maze[row * wid + col] = cell;
    }

    /** Gets the cell at idx in the maze. */
    Cell&
    operator[] (int idx)
    {
        return maze[idx];
    }

    /** Connects cells at idx1 and idx2 */
    void
    connect(unsigned idx1, unsigned idx2)
    {
        int diff = idx1 - idx2; 

        if (diff == 1)
        {
            maze[idx1].setDirection(DIRECTION::LEFT);
            maze[idx2].setDirection(DIRECTION::RIGHT);
        }
        else if (diff == -1)
        {
            maze[idx1].setDirection(DIRECTION::RIGHT);
            maze[idx2].setDirection(DIRECTION::LEFT);
        }
        else if (diff > 1)
        {
            maze[idx1].setDirection(DIRECTION::UP);
            maze[idx2].setDirection(DIRECTION::DOWN);
        }
        else if (diff < -1)
        {
            maze[idx1].setDirection(DIRECTION::DOWN);
            maze[idx2].setDirection(DIRECTION::UP);
        }
    } 

    /** Returns true if going in dir from startIdx would remain in maze bounds */
    bool
    validMove (unsigned startIdx, DIRECTION dir)
    {
        switch(dir)
        {
            case DIRECTION::UP:
                return startIdx - wid > 0;
            case DIRECTION::DOWN:
                return startIdx + wid < size();
            case DIRECTION::LEFT:
                return startIdx % wid - 1 >= 0;
            case DIRECTION::RIGHT:
                return startIdx + 1 < wid;
        }
        return false;
    }
    
    /** Returns the index that results from moving in dir from startIdx. Does not check bounds. */
    unsigned
    getIdx (unsigned startIdx, DIRECTION dir)
    {
        switch(dir)
        {
            case DIRECTION::UP:
                return startIdx - wid;
            case DIRECTION::DOWN:
                return startIdx + wid; 
            case DIRECTION::LEFT:
                return startIdx - 1; 
            case DIRECTION::RIGHT:
                return startIdx + 1;
        }
        return size();
    }

    /** Returns true if the cell at location idx has at least one open face. */
    bool
    hasCell (unsigned idx)
    {
        return maze[idx].val() != 0; 
    }

    /** Returns true if idx is a valid index into this maze. */
    bool 
    hasIndex (unsigned idx)
    {
        return idx < size();
    }

    /** Opens the left wall of the top left cell to create an entrance to the maze. */
    void
    openStart()
    {
        maze[0].setDirection(DIRECTION::LEFT);
    }

    /** Opens the right wall of the bottom right cell to create an exit to the maze. */
    void
    openEnd()
    {
        maze[size() - 1].setDirection(DIRECTION::RIGHT);
    }
};

/** Overloaded output operator. Allows mazes to be manipulated with <<
    
    @param os - the output stream to insert maze's data into
    @param mz - the maze being sent to the ostream
*/
inline std::ostream& 
    operator<<(std::ostream& os, const Maze &mz)
    {

        for(unsigned row = 0; row < mz.len; ++row)
        {   
            for(unsigned col = 0; col < mz.wid; ++col)
            {
                Cell cur = mz.get(row, col);
                os << "[" << cur.up << "," << cur.down << "," << cur.left << "," << cur.right << "] ";
            }
            os << '\n';
        }
        return os;
    } 