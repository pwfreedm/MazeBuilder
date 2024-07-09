from os import urandom
from include.Maze import genEmptyMaze, Maze, genWilsons, Direction

#TODO: give the python maze an __str__ so that you can print it with the built print
def test(m: Maze) -> int:
    str(m)
    return 1

seed = int.from_bytes(urandom(8))
maze: Maze = genEmptyMaze(100000,100000)
genWilsons(maze, 0)

while True:
    test(maze)

print(maze.size())
print("Seed: ", seed)
print("Dimensions: ", maze.wid(), 'x', maze.len(), ' = ', maze.size())
print("Valid move from 1-2? ", maze.validMove(1, 0))
print("Valid move from 1-6? ", maze.validMove(1, 1))
print("Valid idxs 1, 26? ", maze.validIdx(1), ' ', maze.validIdx(27))

print(maze)
print("-------------------------------------------")
maze.print()

