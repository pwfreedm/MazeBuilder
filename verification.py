from include.PyExternBindings import genEmptyMaze, printMaze, genWilsons

#TODO: move seed generation into python, then return the maze after generation. 
#This will allow chaining like print(gen(emptyMaze(), seed))

#TODO: give the python maze an __str__ so that you can print it with the built print
maze = genEmptyMaze(5,5)
genWilsons(maze, 0)
printMaze(maze)
