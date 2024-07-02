from ctypes import *

builder = CDLL("/git/Thesis/shared/Maze.so")

#Cell definition for ctype usage
class Cell (Structure):
    pass
Cell._fields_ = [("up", c_bool), ("down", c_bool), ("left", c_bool), ("right", c_bool)]

#Maze definition for ctype usage
class Maze (Structure):
    pass
Maze._fields_ = [("maze", POINTER(POINTER(Cell))), ("width", c_int32)]