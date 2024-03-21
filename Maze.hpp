/**
 @a - Peter Freedman
 @n - maze.hpp
 This is a header that represents an unweighted, non-directed graph
 in a space efficient manner.
*/

#pragma once

#include <iostream>
#include <type_traits> //is_integral
#include <vector>      //cell storage in maze

//Valid directions
enum DIRECTION { UP, DOWN, LEFT, RIGHT };


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
            case UP:
                up = true; 
                break;
            case DOWN:
                down = true; 
                break;
            case LEFT:
                left = true; 
                break;
            case RIGHT:
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
            case UP:
                up = !up; 
                break;
            case DOWN:
                down = !down; 
                break;
            case LEFT:
                left = !left; 
                break;
            case RIGHT:
                right = !right; 
                break;
        }
    }
};

class Maze
{
    std::vector<Cell> maze; 
    int len;
    int wid;
    
    public:

    friend std::ostream& operator<< (std::ostream& os, const Maze& mz);

    //require dimensions
    Maze() = delete;

    /** Value ctor - Maze of length x width size

        @param length - the number of rows of cells in the maze
        @param width - the number of columns of cells in the maze 
    */
    Maze(int length, int width)
    :maze(length * width), len(length), wid(width)
    {}

    /** Value ctor - Maze of edgeLen * edgeLen size
     * 
     * @param edgeLen - the length both edges of the maze should be.
    */
    Maze(int edgeLen)
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
    int
    length()
    { return len; }
   
    /** const Accessor for length - returns the number of rows of cells in this maze*/
    int
    length() const
    { return len; }

    /** Accessor for width - returns the number of cols of cells in this maze*/
    int
    width()
    { return wid; }

    /** const Accessor for width - returns the number of cols of cells in this maze*/
    int
    width() const
    { return wid; }

    /** Acessor for the size of the maze. Multiplies len and wid fields. */
    int
    size()
    {
        return len * wid;
    }

    /** const accessor for the size of the maze. Multiplies len and wid fields */
    int
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
    get(int row, int col)
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
    get(int row, int col) const 
    {
        return maze[row * wid + col];
    }

    /** Puts a cell into the maze at maze[row][col]
    
        @param row - the row of the maze in which to insert @p cell
        @param col - the column of the maze in which to inser @p cell
        @param cell - the cell to be inserted into the maze
    */
    void
    set(int row, int col, const Cell& cell)
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
    connect(int idx1, int idx2)
    {
        int diff = idx1 - idx2; 

        if (diff == 1)
        {
            maze[idx1].setDirection(LEFT);
            maze[idx2].setDirection(RIGHT);
        }
        else if (diff == -1)
        {
            maze[idx1].setDirection(RIGHT);
            maze[idx2].setDirection(LEFT);
        }
        else if (diff > 1)
        {
            maze[idx1].setDirection(UP);
            maze[idx2].setDirection(DOWN);
        }
        else if (diff < -1)
        {
            maze[idx1].setDirection(DOWN);
            maze[idx2].setDirection(UP);
        }
    } 

    /** Returns true if going in dir from startIdx would remain in maze bounds */
    bool
    validMove (int startIdx, DIRECTION dir)
    {
        switch(dir)
        {
            case UP:
                return startIdx - wid > 0;
            case DOWN:
                return startIdx + wid < size();
            case LEFT: 
                return ((startIdx % wid > 0) && hasIndex(startIdx));
            case RIGHT:
                return (startIdx % wid + 1) < wid;
        }
        return false;
    }
    
    /** Returns the index that results from moving in dir from startIdx. Does not check bounds. */
    int
    getIdx (int startIdx, DIRECTION dir)
    {
        switch(dir)
        {
            case UP:
                return startIdx - wid;
            case DOWN:
                return startIdx + wid; 
            case LEFT:
                return startIdx - 1; 
            case RIGHT:
                return startIdx + 1;
        }
        return size();
    }

    /** Returns true if the cell at location idx has at least one open face. Bounds checked. */
    bool
    hasCell (int idx)
    {

        return hasIndex(idx) ? maze[idx].val() != 0 : false;
    }

    /** Returns true if idx is a valid index into this maze. */
    bool 
    hasIndex (int idx)
    {
        return idx < size();
    }

    /** Opens the left wall of the top left cell to create an entrance to the maze. */
    void
    openStart()
    {
        maze[0].setDirection(LEFT);
    }

    /** Opens the right wall of the bottom right cell to create an exit to the maze. */
    void
    openEnd()
    {
        maze[size() - 1].setDirection(RIGHT);
    }
};

/** Overloaded output operator. Allows mazes to be manipulated with <<
    
    @param os - the output stream to insert maze's data into
    @param mz - the maze being sent to the ostream
*/
inline std::ostream& 
    operator<<(std::ostream& os, const Maze &mz)
    {

        for(int row = 0; row < mz.len; ++row)
        {   
            for(int col = 0; col < mz.wid; ++col)
            {
                Cell cur = mz.get(row, col);
                os << "[" << cur.up << "," << cur.down << "," << cur.left << "," << cur.right << "] ";
            }
            os << '\n';
        }
        return os;
    } 