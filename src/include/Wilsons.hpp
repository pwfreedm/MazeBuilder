#pragma once

#include <set>
#include <random>
#include <iostream>
#include <list>
#include <vector>

#include "Maze.hpp"

class Wilsons
{

   Maze &mz; 
   std::minstd_rand0 r;
   std::set<int> unvisited;
   std::vector<Direction> visited;

public:

   Wilsons () = delete;
   
   //ctor
   Wilsons (Maze&mz, long long int seed);

   //runs the algorithm on the maze it was constructed with
   void run ();
   
   //ostream operator. Prints the underlying maze
   friend std::ostream& operator<<(std::ostream& os, const Wilsons &w);

private:

    // Fills unvisited with all valid indices. Skips 0 because it is set during maze generation.
    std::set<int> genUnvisited ();

    //Picks a random starting index from the set of unvisited indices
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