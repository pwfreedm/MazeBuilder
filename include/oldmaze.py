toString = lib.toString
toString.argtypes = [Maze]
toString.restype = c_char_p





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
getNeighbor.restype = c_int32

getIdx = lib.getIdx
getIdx.argtypes = [Maze, c_int32]
getIdx.restype = Cell

hasIndex = lib.hasIndex
hasIndex.argtypes = [Maze, c_int32]
hasIndex.restype = c_bool