#pragma once

#include <string>

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

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

    /** Get the value of the up field */
    bool getUp();
    /** Get the value of the down field */
    bool getDown();
    /** Get the value of the left field */
    bool getLeft();
    /** Get the value of the right field */
    bool getRight();


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

    std::string str();
};