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

genWilsons = builder.genWilsons
genWilsons.argtypes = [Maze, c_longlong]
genWilsons.restype = Maze

genHuntAndKill = builder.genHuntAndKill
genHuntAndKill.argtypes = [Maze, c_longlong]
genHuntAndKill.restype = Maze

genCellularAutomata = builder.genCellularAutomata
genCellularAutomata.argtypes = [Maze, c_longlong]
genCellularAutomata.restype = Maze

genSeed = builder.genSeed
genSeed.argtypes = []
genSeed.restype = c_longlong

printMaze = lib.printMaze
printMaze.argtypes = [Maze]