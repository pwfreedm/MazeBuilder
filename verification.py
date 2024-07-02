from ctypes import *
from test import *

new = builder.newMaze
new.argtypes = [c_int32, c_int32]
new.restype = Maze

newCell = builder.newPresetCell
newCell.argtypes = [c_bool, c_bool, c_bool, c_bool]
newCell.restype = Cell

maze = new(7, 4)
cell = newCell(c_bool(0), c_bool(1), c_bool(1), c_bool(0))
#builder.set(maze, c_int32(2), c_int32(2), cell)
builder.openStart(maze)
builder.openEnd(maze)
builder.printMaze(maze)
