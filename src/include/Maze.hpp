/**
 @a - Peter Freedman
 @n - maze.hpp
 This is a header that represents an unweighted, non-directed graph
 in a space efficient manner.
*/
#pragma once

#include <iostream> 
#include <sstream>
#include <type_traits>
#include <vector>
#include <memory>
#include <concepts>

#include "Cell.hpp"

template <typename T>
concept CanMaze = requires (T const& maze) {
  { maze[0] } -> std::same_as<Cell&>;
};

/** A maze is a vector of cells and a width */
template<CanMaze Mazeable = std::unique_ptr<Cell[]>>
class Maze
{
    Mazeable mz; 
    int len;
    int wid;

public:

    //force a maze to have dimensions
    Maze () = delete; 

    //dimension ctor, creates a numRows x numCols maze
    Maze (int numRows, int numCols)
    :len(numRows), wid(numCols)
    {
        //because smart pointers don't use default ctors
        if constexpr (std::is_same<Mazeable, std::unique_ptr<Cell[]>>())
        {
            mz = std::make_unique<Cell[]>((numRows * numCols) / 2);
        }
        else if constexpr (std::is_same<Mazeable, std::shared_ptr<Cell[]>>())
        {
            mz = std::make_shared<Cell[]>((numRows * numCols) / 2);
        }
        else 
        {
            mz = Mazeable((numRows * numCols) / 2);
        }
    }

    //move ctor - takes ownership of underlying data
    Maze (Mazeable maze, int numRows, int numCols)
    :mz(std::move(maze)), len(numRows), wid(numCols)
    {}
    
    /** decides if idx is the left or right side of a cell. */
    inline Side getSide(int idx) { return idx & 0b1 ?  R_SIDE : L_SIDE;  }

    //Gets the cell pair at index idx in the maze. Does not check bounds.
    Cell& operator[] (size_t idx) { return mz[int(idx / 2)]; }

    //print a maze
    friend std::ostream& operator<< (std::ostream& os, const Maze<>& mz);

    //Equality op - Compares fields from smallest to largest
    bool operator== (Maze<Mazeable> const &o) const
    {
        return 
        wid != o.wid ? false :
        len != o.len ? false :
        mz != o.mz ? false : true;
    }

    //Inequality op - Compares fields from smallest to largest
    bool operator!= (Maze<Mazeable> const &o) const
    {
        return !(*this == o);
    }

    //Accessor for number of columns of cells in the maze (width)
    int width () { return wid; }

    //Const accessor for number of columns of cells in the maze (width)
    int width () const { return wid; } 

    //Accessor for number of cells in the maze
    int size () { return wid * len; }

    //Const accessor for number of cells in the maze
    int size () const { return wid * len; }

    //Accessor for number of rows of cells in the maze (length)
    int length () { return len; }

    /** Element access operator for a maze. Returns a reference to the element
    at position row * maze width + col
    
    NOTE: No bounds checking is performed

    @param row - the row number of the element 
    @param col - the column number of the element
    */
    Cell& getElem (int row, int col) { 
        int idx = (row * wid + col) / 2; 
        return mz[idx]; 
    }

    /** Element access operator for a maze. Returns a reference to the element
    at position row * maze width + col
    
    NOTE: No bounds checking is performed

    @param row - the row number of the element 
    @param col - the column number of the element
    */
    const Cell& getElem (int row, int col) const { 
            int idx = (row * wid + col) / 2; 
            return mz[idx]; 
    }

    /** Puts a cell into the maze at maze[row][col]

    @param row - the row of the maze in which to insert @p cell
    @param col - the column of the maze in which to inser @p cell
    @param cell - the cell to be inserted into the maze
    */
    void set (int row, int col, const Cell& cell) { (*this)[row * wid + col] = cell; }



    //Connects cells at idx1 and idx2. Bounds checked
    void connect (int idx1, int idx2)
    {
        if (!hasIndex(idx1) || !hasIndex(idx2)) { return; }
        Side src_side = getSide(idx1);
        Side dst_side = getSide(idx2);
        Direction dir = getDirection(idx1, idx2);

        switch(dir)
        {
            case UP:
                (*this)[idx1].setDirection(src_side, UP);
                (*this)[idx2].setDirection(dst_side, DOWN);
                break; 
            case DOWN: 
                (*this)[idx1].setDirection(src_side, DOWN);
                (*this)[idx2].setDirection(dst_side, UP);
                break; 
            case LEFT:
                (*this)[idx1].setDirection(src_side, LEFT);
                (*this)[idx2].setDirection(dst_side, RIGHT);
                break; 
            case RIGHT:
                (*this)[idx1].setDirection(src_side, RIGHT);
                (*this)[idx2].setDirection(dst_side, LEFT);
            default:
                break;
        };
    }

    //Returns true if going in dir from startIdx would remain in maze bounds
    bool validMove (int startIdx, Direction dir)
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
            default:
                break;
        }
        return false;
    }

    /** Returns the index that results from moving in dir from startIdx.
        Returns the size of the maze if the move would be out of bounds 
    */
    int getNeighbor (int startIdx, Direction dir)
    {
        if(!validMove(startIdx, dir)) { return size(); }

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
            default:
                break; 
        }
        return size();
    }

    /** Gets all neighbors to the cell at idx that meet criteria @p connected. 
    
        @p connected - whether or not the neighbors being searched for are connected

        @return - A vector of Direction values containing all the neighbors that 
        are either connected or disconnected
    */
    std::vector<Direction> getNeighbors (int idx, bool connect = true)
    {
        std::vector<Direction> out;
        out.reserve(4);

        for (Direction dir : {UP, DOWN, LEFT, RIGHT})
        {
            if (validMove(idx, dir) && 
                (inMaze(getNeighbor(idx, dir)) == connect))
            {
                out.push_back(dir);
            }
        }
        return out;
    }

    /** Gets all directions that would be valid moves from idx */
    std::vector<Direction> getAllNeighbors (int idx)
    {
        std::vector<Direction> out; 
        out.reserve(4);

        for (Direction dir : {UP, DOWN, LEFT, RIGHT})
        {
            if (validMove(idx, dir))
            {
                out.push_back(dir);
            }
        }
        return out;
    }

    //Returns the direction to get to dstIdx from startIdx
    Direction getDirection (int startIdx, int dstIdx)
    {
        int diff = startIdx - dstIdx; 

        if (diff == 1)
        {
            return LEFT; 
        }
        else if (diff == -1)
        {
            return RIGHT;
        }
        else if (diff > 1)
        {
            return UP;
        }
        else 
        {
            return DOWN;
        }
    }

    //Returns true if the cell at location idx is connected to the maze. Bounds checked.
    bool inMaze (int idx) { return hasIndex(idx) && (*this)[idx].side_val(getSide(idx)); }

    //Returns true if idx is a valid index into this maze
    bool hasIndex (int idx) { return idx < size(); }
    
    //Opens the left wall of the top left cell of the maze to create an entrance
    void openStart() { mz[0].setDirection(L_SIDE, LEFT); }

    //Opens the right wall of the bottom left cell of the maze to create an exit
    void openEnd() { mz[(len * wid / 2) - 1].setDirection(R_SIDE, RIGHT); }

    //returns a string representation of this maze
    std::string toString () const
    {
        std::stringstream ss;
        int len = size() / width();
        for(int row = 0; row < len; ++row)
        {   
            for(int col = 0; col < wid; ++col)
            {
                Cell cur = getElem(row, col);
                ss << cur.str() << " ";
            }
            ss << '\n';
        }
        return ss.str();
    }

};
/** Overloaded output operator. Allows mazes to be manipulated with <<
    
    @param os - the output stream to insert maze's data into
    @param mz - the maze being sent to the ostream
*/
inline std::ostream& 
operator<<(std::ostream& os, const Maze<> &mz)
{
    os << mz.toString();
    return os;
} 