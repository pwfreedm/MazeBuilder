#pragma once


#include <random>
#include <iostream>
#include <map>
#include <span>

#include "Maze.hpp"

class Wilsons
{

   Maze<> &mz; 
   std::minstd_rand0 r;
   // TODO: probably make this an int to int map where key is start and val is
   // dst
   std::map<int, Direction> visited;

public:

   Wilsons () = delete;
   
   //ctor
   template<CanMaze Mazeable>
   Wilsons (Maze<Mazeable> &mz, long long int seed, bool open_ends = true);

   //runs the algorithm on the maze it was constructed with
   void run (bool open_ends);
   
   //ostream operator. Prints the underlying maze
   friend std::ostream& operator<<(std::ostream& os, const Wilsons &w);

private:

    /** Picks a start idx using the following process:
      1) pick a random number. If the cell at that idx has been visited, 
      2) add one to that number. Repeat until an unvisited cell is found
      3) if the end of the maze is reached, check every index from 0 
      4) if no valid index is found, return maze size
    */
    int pickStartIdx();

    /** Performs a random walk on maze mz. 

    NOTE: this method assumes that at least one cell in the maze has 
    been modified, making that the end of the random walk

    @p mz - the maze to be walked
    @p eraseLoops - a bool deciding whether or not loops are erased

    @return a list of indices representing the walk path
    */
    void randomWalk (int startIdx);

    /** Returns a valid index in which a random walk can move. 

    @return - an int representing the cur valid index that can be stepped to
    @return - mz.size() if there are no valid directions to move
    */
    int validStep (int cur);

    //Writes the completed loop-erased walk into the maze that will be returned
    void updateMaze (int startIdx);
};