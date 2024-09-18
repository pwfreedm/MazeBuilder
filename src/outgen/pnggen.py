from png import Writer, Image, from_array
import os
import array
import time

from maze import Maze
#png is encoded in greyscale-alpha, so pixels only have two specifiers
edgelen = 18
ptsize = 3

#default file stuff
default_path = os.path.join(os.path.dirname(__file__), '../', '../', 'output')
default_filename = time.strftime("%Y.%m.%d-%H:%M:%S")

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
    row.append(0)

def add_transparent_px (row: list):
    row.append(0)
    row.append(255)

def make_top_row (rowlen: int):
    for line in range(ptsize):
        pass

def create_file (filepath, filename):
    if not os.path.exists(filepath):
        os.makedirs(filepath)
    
    fullpath = os.path.join(filepath, filename + '.png')
    return open(fullpath, 'wb')

def convert_to_png (mz: Maze, filepath = default_path, filename: str = default_filename):
    rowlen = mz.width * edgelen
    collen = (mz.size() / mz.width) * edgelen

    image = Writer(width=15, height=1, greyscale=True, alpha=True)
    line = []
    for i in range (15):
        add_black_px(line)
    lines = [line]
    image.write(create_file(filepath, filename), lines)


convert_to_png(Maze(1,1))

