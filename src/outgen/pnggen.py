from png import Writer
import os
import time

from maze import Maze
#edgelen is how many pixels the edge of an individual cell is
edgelen = 18
#thickness of any given line in pixels 
ptsize = 2

#default file stuff
default_path = os.path.join(os.path.dirname(__file__), '../', '../', 'output')
default_filename = time.strftime("%Y.%m.%d-%H:%M:%S")

'''
    Maze Width: 15 columns
    Maze Length: 1 column
    
    Line thickness: 3 pixels
    '''
'''General Approach: 
    each line in the output maze will be 3 pixels thick
    each cell will be 18 pixels thick (minus 3 for its top line)

    0) create file and necessary objects
    1) insert the top row of the maze
    2) pull in the first full row of cells
    3) iterate through in 3 pixel chunks
    4) find some smart way to figure out whats black
    5) profit???

    library accepts pngs in the form 
    ([r1p1R, r1p1G, r1p1B, r1p1A, r1p2R, r1p2G, r1p2B, r1p2A]
     [r2p1R, r2p1G, r2p1B, r2p1A, r2p2R, r2p2G, r2p2B, r2p2A])
    and stores them internally as a numpy array for the most part
'''
def add_black_px (row: list):
    row.append(0)
    row.append(255)

def add_transparent_px (row: list):
    row.append(0)
    row.append(0)

def make_top_row (numcols: int) -> list[list[int]]:
    out = []
    for row in range(ptsize):
        line = []
        for col in range(numcols):
            add_black_px(line)
        out.append(line)
    return out

def create_file (filepath, filename):
    if not os.path.exists(filepath):
        os.makedirs(filepath)
    
    fullpath = os.path.join(filepath, filename + '.png')
    return open(fullpath, 'wb')

def convert_to_png (mz: Maze, filepath: str = default_path, filename: str = default_filename):
    numrows = int(mz.width * ptsize)
    numcols = int((mz.size() / mz.width) * edgelen)

    image = Writer(width=numcols, height=numrows, 
                   greyscale=True, alpha=True)
    
    picture_data = make_top_row(numcols)
    

    image.write(create_file(filepath, filename), picture_data)


convert_to_png(Maze(15,1))

