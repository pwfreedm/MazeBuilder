from include.PyExternBindings import genEmptyMaze, printMaze, genWilsons

maze = genEmptyMaze(5,5)
genWilsons(maze, 0)
printMaze(maze)
