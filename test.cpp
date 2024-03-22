
#include <iostream>
//#include <format>

#include "Maze.hpp"

// void
// testCellCtors();

// void
// testCellCompare();

// bool
// testMazeFields(const Maze& m, const int exLen, const int exWid, const bool exEnd);

// void
// testMazeCtors();

// void
// testSizeMutators();

// void
// testGetNPut();

// void
// testPrint();

// void
// testValidStep();

void
testValidMove();

void
testNeighbors();


int main()
{
    // std::cout << "Cell Tests: \n";
    // testCellCtors();
    // testCellCompare();

    // std::cout << "\nMaze Tests: \n";
    // testMazeCtors();
    // testSizeMutators();
    // testGetNPut();
    // testPrint();

    // std::cout << "\nMisc Tests: \n";
    // testValidStep();
    testValidMove();
}

// void
// testCellCtors()
// {
//     Cell dCtor;
//     std::cout << std::format("  Default ctor:   {}\n", dCtor.val() == 0);

//     Cell vCtor(1,1,0,0);
//     std::cout << std::format("  Value Ctor:     {}\n", vCtor.val() == 12);

//     Cell cCtor(vCtor);
//     std::cout << std::format("  Copy Ctor:      {}\n", cCtor.val() == 12);

//     Cell iCtor(1,1,1,0);
//     std::cout << std::format("  Integral Ctor:  {}\n", iCtor.val() == 15);
// }

// void
// testCellCompare()
// {
//     Cell lower(0,0,0,0);
//     Cell higher(0,1,1,0);
//     Cell lowCopy(lower);

//     std::cout << std::format("  Compare lower:  {}\n", lower.compare(higher) < 0);
//     std::cout << std::format("  Compare higher: {}\n", higher.compare(lower) > 0);
//     std::cout << std::format("  Compare equal:  {}\n", lower.compare(lowCopy) == 0);
// }

// bool
// testMazeAccessors(const Maze& m, const int exLen, const int exWid, const bool exEnd)
// {
//     return m.length() == exLen && m.width() == exWid;
// }

// //Maze Ctors are tested using their field accessors.
// void
// testMazeCtors()
// {
//     Maze vCtor(10, 5);
//     std::cout << std::format("  Value Ctor:     {}\n", testMazeAccessors(vCtor, 10, 5, false));

//     Maze cCtor(vCtor);
//     std::cout << std::format("  Copy Ctor:      {}\n", testMazeAccessors(cCtor, 10, 5, false));
// }

void
testValidMove()
{
    Maze test(5,5);
    if (test.validMove(0,UP))
    {
        std::cout << "Issue with validMove up.\n";
    }
    if (test.validMove(24, DOWN))
    {
        std::cout << "Issue with validMove down\n";
    }
    if (test.validMove(0, LEFT))
    {
        std::cout << "Issue with moving left\n";
    }
    if(test.validMove(24, RIGHT))
    {
        std::cout << "Issue with moving right\n";
    }
}