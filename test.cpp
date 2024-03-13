
#include <iostream>
#include <format>

#include "Maze.hpp"

void
testCellCtors();

void
testCellCompare();

bool
testMazeFields(const Maze& m, const int exLen, const int exWid, const bool exEnd);

void
testMazeCtors();

void
testSizeMutators();

void
testGetNPut();

void
testPrint();

void
testValidStep();

int main()
{
    std::cout << "Cell Tests: \n";
    testCellCtors();
    testCellCompare();

    std::cout << "\nMaze Tests: \n";
    testMazeCtors();
    testSizeMutators();
    testGetNPut();
    testPrint();

    std::cout << "\nMisc Tests: \n";
    testValidStep();
}

void
testCellCtors()
{
    Cell dCtor;
    std::cout << std::format("  Default ctor:   {}\n", dCtor.val() == 0);

    Cell vCtor(1,1,0,0);
    std::cout << std::format("  Value Ctor:     {}\n", vCtor.val() == 12);

    Cell cCtor(vCtor);
    std::cout << std::format("  Copy Ctor:      {}\n", cCtor.val() == 12);

    Cell iCtor(15);
    std::cout << std::format("  Integral Ctor:  {}\n", iCtor.val() == 15);
}

void
testCellCompare()
{
    Cell lower(1);
    Cell higher(14);
    Cell lowCopy(lower);

    std::cout << std::format("  Compare lower:  {}\n", lower.compare(higher) < 0);
    std::cout << std::format("  Compare higher: {}\n", higher.compare(lower) > 0);
    std::cout << std::format("  Compare equal:  {}\n", lower.compare(lowCopy) == 0);
}

bool
testMazeAccessors(const Maze& m, const int exLen, const int exWid, const bool exEnd)
{
    return m.length() == exLen && m.width() == exWid;
}

//Maze Ctors are tested using their field accessors.
void
testMazeCtors()
{
    Maze vCtor(10, 5);
    std::cout << std::format("  Value Ctor:     {}\n", testMazeAccessors(vCtor, 10, 5, false));

    Maze cCtor(vCtor);
    std::cout << std::format("  Copy Ctor:      {}\n", testMazeAccessors(cCtor, 10, 5, false));
}

void
testSizeMutators()
{
    Maze vCtor(10, 5);
    vCtor.length(15);
    vCtor.width(10);

    std::cout << std::format("  Length Mutator: {}\n", vCtor.length() == 15);
    std::cout << std::format("  Width Mutator : {}\n", vCtor.width() == 10);
}

void
testGetNPut()
{
    Maze vCtor(2,2);

    std::cout << std::format("  Get:            {}\n", vCtor.get(0,0).val() != 0);

    vCtor.set(0,1,3);
    std::cout << std::format("  Set Value:      {}\n", vCtor.get(0,1).val() == 3);

    vCtor.set(1,0,Cell(1,1,1,1));
    std::cout << std::format("  Set Cell:       {}\n", vCtor.get(1,0).val() == 15);
}

void
testPrint()
{
    Maze vCtor(2,2);
    vCtor.set(0,1,6);
    vCtor.set(1,0,14);
    vCtor.set(1,1,7);

    std::cout << vCtor << '\n';
}