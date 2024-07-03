from include.PyExternBindings import genEmptyMaze, printMaze, newMazeBuilder, genWilsons

builder = newMazeBuilder()
maze = genEmptyMaze(5,5)
genWilsons(builder, maze, 0)
printMaze(maze)
