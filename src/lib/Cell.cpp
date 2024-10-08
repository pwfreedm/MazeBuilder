
#include <sstream> 

#include "../include/Cell.hpp"
/*********************************************************************/
//Cell Definition

Cell::Cell()
:up(0), down(0), left(0), right(0) {}

Cell::Cell(bool up, bool down, bool left, bool right)
: up(up), down(down), left(left), right(right){}

int 
Cell::val() const
{
    return (up << 3) + (down << 2) + (left << 1) + right;
}

bool
Cell::getUp()
{
    return up;
}

bool 
Cell::getDown()
{
    return down;
}

bool
Cell::getRight()
{
    return right;
}

bool 
Cell::getLeft()
{
    return left;
}

bool
Cell::operator== (const Cell &o)
{
    return val() == o.val();
}

bool 
Cell::operator!= (const Cell &o)
{
    return !(*this == o);
}

int
Cell::compare(const Cell &o) const
{
    return val() - o.val();
}

void
Cell::setDirection (Direction dir)
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
        default:
            break;
    }
}

void
Cell::updateDirection(Direction dir)
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
        default:
            break; 
    }
}

std::string
Cell::str()
{
    std::stringstream ss;
    ss << "[" << up << "," << down << "," << left << "," << right << "] ";
    return ss.str();
}