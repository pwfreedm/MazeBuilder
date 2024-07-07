from os import urandom
from ctypes import c_longlong
from include.Maze import genEmptyMaze, Maze, genWilsons, Direction

#TODO: give the python maze an __str__ so that you can print it with the built print
seed = int.from_bytes(urandom(8))
maze: Maze = genEmptyMaze(5,5)
genWilsons(maze, 0)
print(maze.size())
print("Seed: ", seed)
print("Dimensions: ", maze.wid(), 'x', maze.len(), ' = ', maze.size())
print("Valid move from 1-2? ", maze.validMove(1, 0))
print("Valid move from 1-6? ", maze.validMove(1, 1))
print("Valid idxs 1, 26? ", maze.validIdx(1), ' ', maze.validIdx(27))

print(maze)
print("-------------------------------------------")
maze.print()