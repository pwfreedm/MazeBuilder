#include "include/Maze.hpp"
#include "include/Wilsons.hpp"

int main ()
{
    Maze m(10, 10);
    Wilsons w(m, 0);
    w.run();
    std::cout << m;
}