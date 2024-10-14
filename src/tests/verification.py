from mazebuilder import Maze, Cell

def cell_error(idx: int, problem: str, idx2: int = -1):
    if idx2 == -1: 
        print("Problem with cell", idx, ":\n\t", problem)
    else:
        print("Problem between cells", idx, "and", idx2, ":\n\t", problem)

def check_connections(mz: Maze, seed: int, silent: bool = True) -> bool:
    if not silent: 
        print("Checking seed", seed, "size", int(mz.size() / mz.width), "x", mz.width, ":" )

    good: bool = True

    #make sure beginning is present
    if not mz[0].left:
        if silent: return False
        good = False
        cell_error(0, "expected left face to be open (beginning of maze)")

    #make sure end is present
    if not mz[mz.size() - 1].right:
        if silent: return False
        good = False
        cell_error(mz.size() - 1, "expected right face to be open (exit of maze)")

    cur: Cell
    for idx in range(mz.size()):
        cur = mz[idx]

        #make sure top row has no upwards facing connections
        if idx - mz.width < 0 and cur.up:
            if silent: return False
            good = False
            cell_error(idx, "open upwards while on the top row")
        
        #make sure bottom row has no downwards facing connections
        elif idx + mz.width > mz.size() and cur.down:
            if silent: return False
            good = False
            cell_error(idx, "open downwards while on the bottom row")
        
        #make sure left column has no left facing connections
        elif idx % mz.width == 0 and cur.left:
            if idx != 0:    #exception if we are checking where the maze entrance is
                if silent: return False
                good = False
                cell_error(idx, "open on the left while on the left side")
        
        #make sure right column has no right facing connections
        elif (idx + 1) % mz.width == 0 and cur.right:
            if idx != mz.size() - 1:    #exception if we are checking where maze exit is
                if silent: return False
                good = False
                cell_error(idx, "open on the right while on the right side")
        
        #check to make sure this cell and the one to its right agree on their connection
        if cur.right and (idx + 1) % mz.width != 0: 
            if not mz[idx + 1].left: 
                if silent: return False
                good = False
                cell_error(idx, "lower idx has open right face, higher idx has closed left face", idx + 1)
        
        #check to make sure this cell and the one below it agree on their connection
        if cur.down and idx + mz.width < mz.size():
            if not mz[idx + mz.width].up: 
                if silent: return False
                good = False
                cell_error(idx, "lower idx has open bottom face, higher idx has closed top face", idx + mz.width)
    if good and not silent: 
        print("\tmaze is well constructed")
    
    return good


