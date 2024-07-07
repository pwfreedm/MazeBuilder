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
    #TODO: Why is the first byte flipped?
    def __str__ (self):
        return '[' + str(int(not self.up)) + ',' + str(int(self.down)) + ',' + str(int(self.left)) + ',' + str(int(self.right)) + '] '
 
Cell._fields_ = [("up", c_bool), ("down", c_bool), ("left", c_bool), ("right", c_bool)]

class Maze (Structure):

    def __getitem__ (self, key):
        return lib.getIdx(self, c_int32(key))
    
    # def genWilsons (self, seed: int):
    #     builder.genWilsons(self, c_longlong(seed))
    
    # def genHuntAndKill (self, seed: int):
    #     builder.genHuntAndKill(self, c_longlong(seed))

    # def genCellularAutomata (self, seed: int):
    #     builder.genCellularAutomata(self, c_longlong(seed))

    def wid (self) -> int:
        return self.width
    
    def size (self) -> int:
        return lib.size(self)
    
    def len (self) -> int:
        return lib.length(self)
    
    def at (self, row: int, col: int) -> Cell: 
        return Cell(lib.get(self, c_int32(row), c_int32(col)))
    
    def validMove (self, startIdx: int, dir: Direction) -> bool:
        '''bound checks a move in direction dir from startIdx'''
        return lib.validMove(self, c_int32(startIdx), dir)
    
    def getNeighbor (self, startIdx: int, dir: Direction) -> int:
        return lib.getNeighborIdx(self, c_int32(startIdx), dir)
    
    def validIdx (self, idx: int) -> bool:
        '''bound checks an index'''
        return lib.hasIndex(self, c_int32(idx))
    
    def __str__ (self) -> str:
        out: str = ""
        for row in range(0, self.len()):
            for col in range(0, self.width):
                out += str(self.at(row, col))
            out += '\n'
        return out
    
    def print (self):
        lib.printMaze(self)
        
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