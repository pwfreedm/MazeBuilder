from os import urandom
from ctypes import c_longlong
from include.PyExternBindings import genEmptyMaze, printMaze, genWilsons, genSeed

#TODO: move seed generation into python, then return the maze after generation. 
#This will allow chaining like print(gen(emptyMaze(), seed))

#TODO: give the python maze an __str__ so that you can print it with the built print
seed = int.from_bytes(urandom(8))
printMaze(genWilsons(genEmptyMaze(5,5), seed))
print("Seed: ", seed)
 