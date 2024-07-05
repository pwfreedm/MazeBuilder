from ctypes import *
from enum import Enum
from os import urandom

lib = CDLL("/git/Thesis/shared/Maze.so")
builder = CDLL("/git/Thesis/shared/MazeBuilding.so")

################## Class Definitions ##################

class Direction (Enum):
    up = 0
    down = 1
    left = 2
    right = 3

    @classmethod
    def from_param(cls, obj):
        return int(obj)
    
class Cell (Structure):
    pass
Cell._fields_ = [("up", c_bool), ("down", c_bool), ("left", c_bool), ("right", c_bool)]

class Maze (Structure):

    def genWilsons (self, seed: int):
        builder.genWilsons(self, c_longlong(seed))
    
    def genHuntAndKill (self, seed: int):
        builder.genHuntAndKill(self, c_longlong(seed))

    def genCellularAutomata (self, seed: int):
        builder.genCellularAutomata(self, c_longlong(seed))

    def width (self) -> int:
        return self.width
    
    def size (self) -> int:
        return lib.size(self)
    
    def length (self) -> int:
        return lib.length(self)
    
    def at (self, row: int, col: int) -> Cell: 
        return lib.get(self, c_int32(row), c_int32(col))
    
    
    def print (self):
        builder.printMaze(self)
        
Maze._fields_ = [("maze", POINTER(Cell)), ("width", c_int32)]

#TODO: figure out how to make this into an __init__ 
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

printMaze = lib.printMaze
printMaze.argtypes = [Maze]

getWidth = lib.width
getWidth.argtypes = [Maze]
getWidth.restype = c_int32

getSize = lib.size
getSize.argtypes = [Maze]
getSize.restype = c_int32

getLength = lib.length
getLength.argtypes = [Maze]
getLength.restype = c_int32

getCell = lib.get
getCell.argtypes = [Maze, c_int32, c_int32]
getCell.restype = Cell

validMove = lib.validMove
validMove.argtypes = [Maze, c_int32, Direction]
validMove.restype = c_bool

getNeighbor = lib.getNeighborIdx
getNeighbor.argtypes = [Maze, c_int32, Direction]
getNeighbor.restype = int

getIdx = lib.getIdx
getIdx.argtypes = [Maze, c_int32]
getIdx.restype = Cell

hasIndex = lib.hasIndex
hasIndex.argtypes = [Maze, c_int32]
hasIndex.restype = c_bool