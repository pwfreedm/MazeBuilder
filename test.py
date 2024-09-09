from Maze import Maze, Cell

mz = Maze(5,3)
mz[1, 2] = Cell(1,0,0,1)
print(str(mz))