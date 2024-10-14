#pragma once


#include <random>
#include <iostream>
#include <map>

#include "Maze.hpp"

template<CanMaze Mazeable = std::unique_ptr<Cell[]>>
class Wilsons
{

   Maze<Mazeable> &mz; 
   std::minstd_rand0 r;
   // TODO: probably make this an int to int map where key is start and val is
   // dst
   std::map<int, Direction> visited;

public:

   Wilsons () = delete;
   
   //ctor
   Wilsons (Maze<Mazeable> mz, long long int seed, bool open_ends = true)
   :mz(mz), r(seed)
   {
      run(open_ends);
   }

   //runs the algorithm on the maze it was constructed with
   void run (bool open_ends)
   {
      mz.openStart();

      int startIdx = pickStartIdx();
      while (startIdx != mz.size())
      {
         visited.clear();
         randomWalk(startIdx);
         updateMaze(startIdx);
         startIdx = pickStartIdx();
      }
      
      if (!open_ends) { mz[0].updateDirection(L_SIDE, LEFT); }
      else { mz.openEnd(); }
   }
   
   //ostream operator. Prints the underlying maze
   template<CanMaze MazeType> 
   friend std::ostream& operator<<(std::ostream& os, const Wilsons<MazeType> &w);

private:

    /** Picks a start idx using the following process:
      1) pick a random number. If the cell at that idx has been visited, 
      2) add one to that number. Repeat until an unvisited cell is found
      3) if the end of the maze is reached, check every index from 0 
      4) if no valid index is found, return maze size
    */
    int pickStartIdx()
    {
      int setIdx = r() % mz.size();
      for (int i = setIdx; i < mz.size(); ++i)
      {
         if (!mz.inMaze(i)) { return i; }
      }

      for (int i = 0; i < setIdx; ++i)
      {
         if (!mz.inMaze(i)) { return i; }
      }

      return mz.size();
    }

    /** Performs a random walk on maze mz. 

    NOTE: this method assumes that at least one cell in the maze has 
    been modified, making that the end of the random walk

    @p mz - the maze to be walked
    @p eraseLoops - a bool deciding whether or not loops are erased

    @return a list of indices representing the walk path
    */
    void randomWalk (int startIdx)
    {
      int cur = startIdx; 
      int prev;
      
      while(!mz.inMaze(cur))
      {
         prev = cur; 
         cur = validStep(cur);
         visited[prev] = mz.getDirection(prev, cur);
      }
    }

    /** Returns a valid index in which a random walk can move. 

    @return - an int representing the cur valid index that can be stepped to
    @return - mz.size() if there are no valid directions to move
    */
    int validStep (int cur)
    {
      std::vector<Direction> steps;
      steps.reserve(4);

      for(Direction dir : {UP, DOWN, LEFT, RIGHT})
      {
         if (mz.validMove(cur, dir))
         {
               steps.push_back(dir);
         }
      }
      
      int nextDir = r() % steps.size();
      return mz.getNeighbor(cur, steps[nextDir]);
    }

    //Writes the completed loop-erased walk into the maze that will be returned
    void updateMaze (int startIdx)
    {
      int curIdx = startIdx; 
      int nextIdx = mz.getNeighbor(startIdx, visited[curIdx]);

      while (visited.contains(mz.getNeighbor(curIdx, visited[curIdx])))
      {
         mz.connect(curIdx, nextIdx);
         curIdx = nextIdx; 
         nextIdx = mz.getNeighbor(curIdx, visited[curIdx]);
      }
      mz.connect(curIdx, nextIdx);
    }
};

template <CanMaze Mazeable> 
inline std::ostream& 
operator<<(std::ostream& os, const Wilsons<Mazeable> &w)
{
    os << w.mz;
    return os;
}