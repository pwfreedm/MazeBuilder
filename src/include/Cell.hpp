#pragma once

#include <cstdint>
#include <string>
#include <sstream>

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

/** Basically an enum, but done with defs to avoid explicit int conversions
    in the Cell API. 

    when passing parameters of type Side, please use the macros 
        L_SIDE and R_SIDE
*/
typedef int Side;
#define L_SIDE 4
#define R_SIDE 0

struct Cell
{
    std::uint8_t walls : 8;

    public: 
    
    //default ctor
    Cell ()
    :walls(0b00000000)
    {}

    /** Returns an int representation of this node by treating the underlying
        bool as a single byte int
    */
    inline int val () { return walls; }
    inline int val (Side l_or_r) { return get_side(l_or_r); }

    /** returns the value of Direction dir in cell l_or_r */
    inline bool check_direction(Side l_or_r, Direction dir) { return (walls >> (l_or_r + dir)) & 0b1; }

    /** Get the value of the up field */
    inline bool up(Side l_or_r) { return check_direction(l_or_r, UP); }
    /** Get the value of the down field */
    inline bool down(Side l_or_r) { return check_direction(l_or_r, DOWN); }
    /** Get the value of the left field */
    inline bool left(Side l_or_r) { return check_direction(l_or_r, LEFT); }
    /** Get the value of the right field */
    inline bool right(Side l_or_r) { return check_direction(l_or_r, RIGHT); }

    /** Read Only Accessor for either the left or right side, returns option supplied */
    inline bool get_side(Side l_or_r) { return (walls >> l_or_r) & 0b1111; }

    /** Accessor for a pair of cells. Returns them as a single bool, where the
        lower 4 bits are the right side and the upper 4 bits are the left side 
     */
    inline bool get_pair() { return walls; }

    
    /** Compares this cell to another cell

    @p o - the cell to compare to this one

    @return - 0 if their fields are set the same
    @return - <0 if o.val > this.val
    @return - >0 if this.val > o.val
    */
    inline int compare (Cell &o) { return this->val() - o.val(); }

    //checks if two cells have the same value
    inline bool operator== (Cell &o) { return compare(o) == 0; }

    //checks if two cells have different values
    inline bool operator!= (Cell &o) { return !(*this == o); }

    // Sets direction dir to true (removes that wall from the cell)
    inline void setDirection (Side l_or_r, Direction dir) { walls |= (0b1 << (l_or_r + dir)); }

    //Flips the state of the bit corresponding to dir
    inline void updateDirection (Side l_or_r, Direction dir) { walls ^= (0b1 << (l_or_r + dir)); }

    /** returns a string representation of this cell */
    inline std::string str()
    {
        //TODO: expand this to take L or R and only return the side requested
        std::stringstream ss;
        ss << val();
        return ss.str();
    }
};