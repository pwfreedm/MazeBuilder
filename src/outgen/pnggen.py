from png import Writer
import os
import time

from maze import Maze
#edgewid is how many pixels the edge of an individual cell is
edgewid = 12
edgelen = edgewid - 2
#thickness of any given line in pixels 
ptsize = 2

#default file stuff
default_path = os.path.join(os.path.dirname(__file__), '../', '../', 'output')
default_filename = time.strftime("%Y.%m.%d-%H:%M:%S")

def add_black_px (row: list, count: int = 1):
    '''adds a black pixel to the provieded list'''
    row.append([0, 255] * count)


def add_transparent_px (row: list, count: int = 1):
    '''adds a transparent pixel to the provided list'''
    row.append([0, 0] * count)


def make_top_row (numcols: int) -> list[list[int]]:
    '''makes the top row of the maze (inserts ptsize lines of black)'''
    out = []
    for row in range(ptsize):
        line = []
        for col in range(numcols):
            add_black_px(line)
        out.append(line)
    return out

def create_file (filepath, filename):
    ''' creates a file given a filepath and a file name
        returns the open file object '''
    if not os.path.exists(filepath):
        os.makedirs(filepath)
    
    fullpath = os.path.join(filepath, filename + '.png')
    return open(fullpath, 'wb')

def encode_row (mz: Maze, start_idx: int) -> list[int]:
    '''Reads a row from the maze and converts it to a format 
        that will be more useful in png generation

    '''
    encoded = []
    for pos in range(mz.width):
        cur = mz[start_idx + pos]
        if cur.right and not cur.down:
            encoded.append(1)
        elif cur.down and not cur.right:
            encoded.append(2)
        elif cur.right and cur.down:
            encoded.append(3)
        else:
            encoded.append(0)
    return encoded

def pngify_row (row: list[int]) -> list[int]:
    out = []
    for idx in range(len(row)):
        if row[idx] > 1:
            add_transparent_px(out, edgewid)
        else:
            add_transparent_px(out, edgewid - ptsize)
            add_black_px(out, ptsize)
    return out

def handle_bottom_row (row: list[int]) -> list[int]:
    out = []
    for idx in range(len(row)):
        if row[idx] < 2: 
            add_black_px(out, edgewid)
        else: 
            add_transparent_px(out, edgewid)
    return out

def convert_to_png (mz: Maze, filepath: str = default_path, filename: str = default_filename):
    numrows = int(mz.width * ptsize)
    numcols = int((mz.size() / mz.width) * edgewid)

    image = Writer(width=numcols, height=numrows, 
                   greyscale=True, alpha=True)
    
    picture_data = make_top_row(numcols)

    for row in range(numrows):
        cur_row = encode_row(mz, row * mz.width)
        for _ in range(edgewid - 1):
            picture_data.append(pngify_row(cur_row))
        picture_data.append(handle_bottom_row(cur_row))
    
    #making the left boundary of the maze
    for row in range(edgewid, numrows):
        for idx in range(1, ptsize * 2, 2):
            picture_data[row][idx] = 255
    
    image.write(create_file(filepath, filename), picture_data)


convert_to_png(Maze(15,1))

