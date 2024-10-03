#include "../include/Maze.hpp"
#include <sstream> 
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

/*********************************************************************/
//Maze Definition

Maze::Maze(int length, int width)
:maze(length * width), wid(width)
{}

Maze::Maze(std::vector<Cell> &mz, int width)
:maze(std::move(mz)), wid(width)
{}

Maze::Maze(int edgeLen)
:maze(edgeLen * edgeLen), wid(edgeLen)
{}

std::vector<Cell> Maze::getMaze()
{
    return maze;
}

Cell* Maze::data()
{
    return maze.data();
}

bool
Maze::operator== (Maze& o)
{
    return 
            wid != o.wid ? false :
            maze.data() != o.maze.data() ? false : true;
}

bool
Maze::operator!= (Maze &o)
{
    return !(*this == o);
}

int
Maze::width()
{ return wid; }

int
Maze::width() const
{ return wid; }

int
Maze::size()
{
    return maze.size();
}

int
Maze::size() const
{
    return maze.size();
}

int
Maze::length()
{
    return maze.size() / wid;
}

Cell&
Maze::get(int row, int col)
{
    return maze[row * wid + col];
}

const Cell&
Maze::get(int row, int col) const 
{
    return maze[row * wid + col];
}

void
Maze::set(int row, int col, const Cell& cell)
{
    maze[row * wid + col] = cell;
}

Cell&
Maze::operator[] (int idx)
{
    return maze[idx];
}

void
Maze::connect(int idx1, int idx2)
{
    if (!hasIndex(idx1) || !hasIndex(idx2)) { return; }

    Direction dir = getDirection(idx1, idx2);

    switch(dir)
    {
        case UP:
            maze[idx1].setDirection(UP);
            maze[idx2].setDirection(DOWN);
            break; 
        case DOWN: 
            maze[idx1].setDirection(DOWN);
            maze[idx2].setDirection(UP);
            break; 
        case LEFT:
            maze[idx1].setDirection(LEFT);
            maze[idx2].setDirection(RIGHT);
            break; 
        case RIGHT:
            maze[idx1].setDirection(RIGHT);
            maze[idx2].setDirection(LEFT);
        default:
            break;
    };
} 

bool
Maze::validMove (int startIdx, Direction dir)
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

int
Maze::getNeighbor (int startIdx, Direction dir)
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

std::vector<Direction>
Maze::getNeighbors (int idx, bool connect)
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

std::vector<Direction> 
Maze::getAllNeighbors(int idx)
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

Direction
Maze::getDirection (int startIdx, int dstIdx)
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

bool
Maze::inMaze (int idx)
{
    return hasIndex(idx) ? maze[idx].val() != 0 : false;
}

bool 
Maze::hasIndex (int idx)
{
    return idx < size();
}

void
Maze::openStart()
{
    maze[0].setDirection(LEFT);
}

void
Maze::openEnd()
{
    maze[size() - 1].setDirection(RIGHT);
}

std::string
Maze::toString() const
{
    std::stringstream ss;
    int len = size() / width();
    for(int row = 0; row < len; ++row)
    {   
        for(int col = 0; col < wid; ++col)
        {
            Cell cur = get(row, col);
            ss << "[" << cur.up << "," << cur.down << "," << cur.left << "," << cur.right << "] ";
        }
        ss << '\n';
    }
    return ss.str();
}

/** Overloaded output operator. Allows mazes to be manipulated with <<
    
    @param os - the output stream to insert maze's data into
    @param mz - the maze being sent to the ostream
*/
inline std::ostream& 
operator<<(std::ostream& os, const Maze &mz)
{
    os << mz.toString();
    return os;
} 