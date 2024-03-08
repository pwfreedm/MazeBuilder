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
#include <chrono>      //time
#include <random>      //randomization where needed
#include <queue>       //wilson's uses a queue for building paths
#include <list>

/**A cell in the maze consists of 5 bytes, [up, down, left, right, seen]
    By default, all five values are initialized to false. Seen is used
    for tracking whether or not the cell is already included in a random walk
    without modifying its connections (which is the criteria for it being 
    considered a permanent part of the maze)
*/
struct Cell 
{
    bool up{0};
    bool down{0};
    bool left{0};
    bool right{0};
    //this is so that random walks know if the cell has been seen without
    //having been made part of the maze yet
    bool seen{0};

    //Default ctor
    Cell()
    {}

    //Value ctor
    Cell(bool up, bool down, bool left, bool right)
    : up(up), down(down), left(left), right(right){}

    //Copy ctor
    Cell(const Cell &n)
    :up(n.up), down(n.down), left(n.left), right(n.right), seen(n.seen){}

    /** Integral constructor for a node. Takes the lowest 4 bits of the Integral type 
        and treats them as bools. 
        
        NOTE: The fourth bit is assigned to up, third to down, second to left
            and first to right.
        
        NOTE: behavior undefined for integrals with less than four bits
        
        EX: 59 in binary is 0b111011. To convert it to a node, do the following:
            1) take 0b111011 & 1000 - this returns 1, so up is set to 1
            2) take 0b111011 & 100  - this returns 0, so down is set to 0
            3) take 0b111011 & 10   - this returns 1, so left is set to 1
            4) take 0b111011 & 1    - this returns 1, so right is set to 1
            
            Thus the resulting node can be accessed from above and either side
            but not from below.
    */
    template<typename T>
    requires (std::is_integral_v<T>)
    Cell(const T n)
    :up(n & 0b1000), down(n & 0b100), left(n & 0b10), right(n & 0b1){} 

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
        return val() == o.val() && seen == o.seen;
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

    //marks a cell as seen
    void
    isSeen()
    {
        seen = 1;
    }

    //marks a cell as unseen (for loop erasing)
    void
    unSeen()
    {
        seen = 0;
    }

    /** Accessor for seen */
    bool
    checkSeen()
    {
        return seen;
    }

    void
    setLeft ()
    {
        left = 1;
    }

    void 
    setRight ()
    {
        right = 1;
    }

    void 
    setUp ()
    {
        up = 1;
    }

    void 
    setDown ()
    {
        down = 1;
    }

    /** Sets the value of this cell to the supplied param if param != val().

        @param val - the new value for this cell to have

        NOTE: This is done by taking the lowest four bits of the param supplied
        NOTE: Marks the cell as seen as well
    */
    template<typename T>
    requires(std::is_integral_v<T>)
    void
    set(const T val)
    {
        if(val != this->val())
        {
            up = val & 0b1000;
            down = val & 0b100; 
            left = val & 0b10;
            right = val & 0b1; 
        }
    }
};

class Maze
{
    std::vector<Cell> maze; 
    unsigned len;
    unsigned wid;

    

    public:

    friend class Wilsons;
    friend std::ostream& operator<< (std::ostream& os, const Maze& mz);

    Maze() = delete;

    /** Value cto - Reserves a vector of length x width size

        Inserts a cell with the left and right sides open to serve as the 
        beginning of the maze 
        
        @param length - the number of rows of cells in the maze
        @param width - the number of columns of cells in the maze 
    */
    Maze(unsigned length, unsigned width)
    :maze(length * width), len(length), wid(width)
    {
        maze[0].set(3);
    }

    /** Value ctor - Reserves a vector of edgeLen * edgeLen size
     * 
     * Inserts a cell with the left and right sides open to serve as
     *      the beginning of the maze
     * 
     * @param edgeLen - the length both edges of the maze should be.
    */
    Maze(unsigned edgeLen)
    :maze(edgeLen * edgeLen), len(edgeLen), wid(edgeLen)
    {
        maze[0].set(3);
    }

    /** Copy ctor - makes this identical to o by copying over each field
        
        @param o - the maze to make this one a copy of
    */
    Maze(const Maze &o)
    :maze(o.len * o.wid), len(o.len), wid(o.wid)
    {
        maze.reserve(o.len * o.wid);
        maze = o.maze;
    }

    /** Equality op - Compares fields from smallest to largest*/
    bool
    operator== (Maze& o)
    {
        return 
               len != o.len ? false :
               wid != o.wid ? false :
               maze.data() != o.maze.data() ? false : true;
    }

    bool
    operator!= (Maze &o)
    {
        return !(*this == o);
    }

    /** Assignment op - assigns field by field, from smallest to largest*/
    Maze&
    operator= (Maze& o)
    {
        if(this == &o)
        {
            return *this;
        }

        len = o.len;
        wid = o.wid;
        maze = o.maze;

        return *this;
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

    /** Mutator for length - changes the number of rows of cells in the maze
        
        NOTE: This method does not change the maze itself in any way.

        @param newLen - the new row count of this maze
    */
    void
    length(unsigned newLen)
    { 
        len = newLen; 
    }

    /** Mutator for length - changes the number of cols of cells in the maze
    
        NOTE: This method does not change the maze itself in any way

        @param newWid - the new column count of this maze
    */
    void
    width(unsigned newWid)
    { 
        wid = newWid; 
    }

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

    /** Puts a cell into the maze at maze[row][col]
    
        @param row - the row of the maze in which to insert @p cell
        @param col - the column of the maze in which to inser @p cell
        @param value - the numeric value of the cell to be inserted

        NOTE: This is accomplished using the cell's value ctor - reference
        that documentation to understand usage in more depth.
    */
    void
    set(unsigned row, unsigned col, int value)
    {
        maze[row * wid + col].set(value);
    }

    /** Gets the cell at idx in the maze. */
    Cell&
    operator[] (int idx)
    {
        return maze[idx];
    }

    //Connects cells idx1 and idx2 in this maze
    void
    connect(unsigned idx1, unsigned idx2)
    {
        int diff = idx1 - idx2; 

        if (diff == 1)
        {
            maze[idx1].setLeft();
            maze[idx2].setRight();
        }
        else if (diff == -1)
        {
            maze[idx1].setRight();
            maze[idx2].setLeft();
        }
        else if (diff > 1)
        {
            maze[idx1].setUp();
            maze[idx2].setDown();
        }
        else if (diff < -1)
        {
            maze[idx1].setDown();
            maze[idx2].setUp();
        }
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