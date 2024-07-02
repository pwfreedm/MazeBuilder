/**
 @a - Peter Freedman
 @n - maze.hpp
 This is a header that represents an unweighted, non-directed graph
 in a space efficient manner.
*/
#pragma once

#include <iostream> 
#include <vector>

enum Direction { UP, DOWN, LEFT, RIGHT };

/**A cell in the maze consists of 4 bits, [up, down, left, right]
    By default, all four values are initialized to false. 
*/
struct Cell
{
    bool up    : 1;
    bool down  : 1;
    bool left  : 1;
    bool right : 1;

    //default ctor
    Cell ();

    //value ctor
    Cell (bool up, bool down, bool left, bool right);

    /** Returns an int representation of this node using the following method:
        (up << 3) + (down << 2) + (left << 1) + right
    */
    int val () const;

    /** Compares this cell to another cell

    @p o - the cell to compare to this one

    @return - 0 if their fields are set the same
    @return - <0 if o.val > this.val
    @return - >0 if this.val > o.val
    */
    int compare (const Cell &o) const;

    //checks if two cells have the same value
    bool operator== (const Cell &o);

    //checks if two cells have different values
    bool operator!= (const Cell &o);

    // Sets direction dir to true (removes that wall from the cell)
    void setDirection (Direction dir);

    //Flips the state of the bit corresponding to dir
    void updateDirection (Direction dir);
};

/** A maze is a vector of cells and a width */
class Maze
{
    std::vector<Cell> maze; 
    int wid;

    friend class MazeBuilder;

public:

    //force a maze to have dimensions
    Maze () = delete; 

    //dimension ctor, creates a length x width maze
    Maze (int length, int width);

    //shortcut to create a square maze with dimensions edgeLen x edgeLen
    Maze (int edgeLen);

    //print a maze
    friend std::ostream& operator<< (std::ostream& os, const Maze& mz);

    //Equality op - Compares fields from smallest to largest
    bool operator== (Maze& o);

    //Inequality op - Compares fields from smallest to largest
    bool operator!= (Maze &o);

    //Accessor for number of columns of cells in the maze (width)
    int width ();

    //Const accessor for number of columns of cells in the maze (width)
    int width () const; 

    //Accessor for number of cells in the maze
    int size ();

    //Const accessor for number of cells in the maze
    int size () const;

    /** Element access operator for a maze. Returns a reference to the element
    at position row * maze width + col
    
    NOTE: No bounds checking is performed

    @param row - the row number of the element 
    @param col - the column number of the element
    */
    Cell& get (int row, int col);

    /** Element access operator for a maze. Returns a reference to the element
    at position row * maze width + col
    
    NOTE: No bounds checking is performed

    @param row - the row number of the element 
    @param col - the column number of the element
    */
    const Cell& get (int row, int col) const;

    /** Puts a cell into the maze at maze[row][col]

    @param row - the row of the maze in which to insert @p cell
    @param col - the column of the maze in which to inser @p cell
    @param cell - the cell to be inserted into the maze
    */
    void set (int row, int col, const Cell& cell);

    //Gets the cell at index idx in the maze. Does not check bounds.
    Cell& operator[] (int idx);

    //Connects cells at idx1 and idx2. Bounds checked
    void connect (int idx1, int idx2);

    //Returns true if going in dir from startIdx would remain in maze bounds
    bool validMove (int startIdx, Direction dir);

    /** Returns the index that results from moving in dir from startIdx.
        Returns the size of the maze if the move would be out of bounds 
    */
    int getNeighbor (int startIdx, Direction dir);

    /** Gets all neighbors to the cell at idx that meet criteria @p connected. 
    
        @p connected - whether or not the neighbors being searched for are connected

        @return - A vector of Direction values containing all the neighbors that 
        are either connected or disconnected
    */
    std::vector<Direction> getNeighbors (int idx, bool connect = true);

    //Returns the direction to get to dstIdx from startIdx
    Direction getDirection (int startIdx, int dstIdx);

    //Returns true if the cell at location idx is connected to the maze. Bounds checked.
    bool inMaze (int idx);

    //Returns true if idx is a valid index into this maze
    bool hasIndex (int idx);
    
    //Opens the left wall of the top left cell of the maze to create an entrance
    void openStart();

    //Opens the right wall of the bottom left cell of the maze to create an exit
    void openEnd();
};
