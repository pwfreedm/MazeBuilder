from os import urandom
from ctypes import c_longlong
from include.PyExternBindings import genEmptyMaze, printMaze, genWilsons, Maze

#TODO: give the python maze an __str__ so that you can print it with the built print
seed = int.from_bytes(urandom(8))
maze = genEmptyMaze(5,5)
maze.print()
print(maze.size())
print("Seed: ", seed)
 