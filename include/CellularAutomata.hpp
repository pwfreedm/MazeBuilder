#pragma once

#include <random>
#include <vector>
#include <map>

#include "Maze.hpp"

enum State 
{
    UNVISITED,
    CAN_CONNECT, 
    CONNECTED
};

struct Transition
{
    State destination;
    double t_prob;
    int t_forced; 
};

struct Transition_Table
{};

class CellularAutomata
{

    Maze& mz;
    std::minstd_rand r;

    CellularAutomata(Maze& mz, long long int seed);
};