from ctypes import *

lib = CDLL("/git/Thesis/shared/Maze.so")
builder = CDLL("/git/Thesis/shared/MazeBuilder.so")

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

################## MazeBuilder Bindings ##################

newMazeBuilder = builder.newMazeBuilder
newMazeBuilder.argtypes = []
newMazeBuilder.restype = MazeBuilder

genEmptyMaze = builder.genEmptyMaze
genEmptyMaze.argtypes = [c_int32, c_int32]
genEmptyMaze.restype = Maze

getLastSeed = builder.getLastSeed
getLastSeed.argtypes = []
getLastSeed.restype = c_longlong

genWilsons = builder.genWilsons
genWilsons.argtypes = [MazeBuilder, Maze, c_longlong]

genHuntAndKill = builder.genHuntAndKill
genHuntAndKill.argtypes = [MazeBuilder, Maze, c_longlong]

genCellularAutomata = builder.genCellularAutomata
genCellularAutomata.argtypes = [MazeBuilder, Maze, c_longlong]

################## Maze Bindings ##################

printMaze = lib.printMaze
printMaze.argtypes = [Maze]