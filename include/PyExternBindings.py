from ctypes import *

lib = CDLL("/git/Thesis/shared/Maze.so")
builder = CDLL("/git/Thesis/shared/MazeBuilding.so")

################## Class Definitions ##################

class Cell (Structure):
    pass
Cell._fields_ = [("up", c_bool), ("down", c_bool), ("left", c_bool), ("right", c_bool)]

class Maze (Structure):
    pass
Maze._fields_ = [("maze", POINTER(POINTER(Cell))), ("width", c_int32)]

class MazeBuilder (Structure):
    pass
MazeBuilder._fields_ = [("seed", c_longlong)]

genEmptyMaze = builder.genEmptyMaze
genEmptyMaze.argtypes = [c_int32, c_int32]
genEmptyMaze.restype = Maze

#updates provided maze and returns seed
genWilsons = builder.genWilsons
genWilsons.argtypes = [Maze, c_longlong]

#updates provided maze and returns seed
genHuntAndKill = builder.genHuntAndKill
genHuntAndKill.argtypes = [Maze, c_longlong]

#updates provided maze and returns seed
genCellularAutomata = builder.genCellularAutomata
genCellularAutomata.argtypes = [Maze, c_longlong]

printMaze = lib.printMaze
printMaze.argtypes = [Maze]