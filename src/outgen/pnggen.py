from png import Writer, Image
import os
import time

from maze import Maze

#edgewid is how many pixels the edge of an individual cell is
edgewid = 12

#thickness of any given line in pixels 
ptsize = 2

#bytes per pixel. Does not change
bpp = 2

black_alpha = 255
transparent_alpha = 0
#default file stuff
default_path = os.path.join(os.path.dirname(__file__), '../', '../', 'output')
default_filename = time.strftime("%Y.%m.%d-%H:%M:%S")

def add_black_px (row: list, count: int = 1) -> list[int]:
    '''adds a black pixel to the provieded list'''
    row.append([0, 255] * count)
    return row

def add_transparent_px (row: list, count: int = 1) -> list[int]:
    '''adds a transparent pixel to the provided list'''
    row.append([0, 0] * count)
    return row

def create_file (filepath, filename):
    ''' creates a file given a filepath and a file name
        returns the open file object '''
    if not os.path.exists(filepath):
        os.makedirs(filepath)
    
    fullpath = os.path.join(filepath, filename + '.png')
    return open(fullpath, 'ab')

def boundary_row(mz: Maze, row_num: int, top_row: bool = True) -> list[int]:

    out = add_transparent_px([], (edgewid * mz.width))

    start = row_num * mz.width
    for cell in range(start, start + mz.width):
        if (top_row and not mz[cell].up) or (mz[cell].down and not top_row):
            px_start = cell * edgewid * bpp
            for px in range(px_start + 1, px_start + edgewid * bpp, 2):
                out[px] = black_alpha

    return out

def middle_row(mz: Maze, row_num: int) -> list[int]:
    out = add_transparent_px([], (edgewid * mz.width))
    start = row_num * mz.width
    for cell in range(start, start + mz.width):
        if mz[cell].left:
            px_start = cell * edgewid * bpp
            for px in range(px_start + 1, px_start + edgewid * bpp, 2):
                out[px] = black_alpha
        if mz[cell].right:
            pass
    
    return out


def pngify_row(mz: Maze, row_num: int) -> list[list[int]]:
    '''converts one row of maze cells into a chunk of png data'''
    cell_row = []
    
    for _ in range(int(ptsize / 2)):
        cell_row.append(boundary_row(mz, row_num))

    #each middle row is identical to each other middle row, create one and copy.
    next_row = add_transparent_px([], (edgewid * mz.width))
    for _ in range(edgewid - ptsize):
        cell_row.append(next_row)
    
    for _ in range(int(ptsize / 2)):
        cell_row.append(boundary_row(mz, top_row=False))
    
    return cell_row

def convert_to_png (mz: Maze, filepath: str = default_path, filename: str = default_filename):
    numrows = edgewid * mz.length()
    numcols = edgewid * mz.width

    image = Writer(width=numcols, height=numrows, 
                   greyscale=True, alpha=True)
    
    image_array: list[list[int]] = [[]]
    for row in range(numrows):
        image_array += pngify_row(mz, row)
        
    image.write(create_file(filepath, filename), None)

convert_to_png(Maze(15,1))

