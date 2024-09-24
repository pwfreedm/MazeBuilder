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

#these are all just named values for readability
black_px = [0, 255]
transparent_px = [0, 0]

def close_top_face () -> list[int]:
    return black_px * edgewid

def close_bottom_face() -> list[int]:
    return black_px * edgewid

def close_left_face() -> list[int]:
    return (black_px * ptsize) + transparent_px * (edgewid - ptsize)

def close_right_face() -> list[int]:
    return (transparent_px * (edgewid - ptsize)) + black_px * ptsize

def close_lr_face() -> list[int]:
    return black_px * ptsize + transparent_px * (edgewid - 2 * ptsize) + black_px * ptsize

def boundary_row(mz: Maze, row_num: int, top_row: bool = True) -> list[int]:
    out = []

    for idx in range(mz.width):
        cell = mz[(mz.width * row_num) + idx]
        if top_row and not cell.up: 
            out += close_top_face()
        if not top_row and not cell.down: 
            out += close_bottom_face()
    
    print("Boundary: ", len(out))
    return out

def middle_row(mz: Maze, row_num: int) -> list[int]:
    out = []

    for idx in range(mz.width):
        cell = mz[(mz.width * row_num) + idx]
        if not cell.right and not cell.left: 
            out += close_lr_face()
        elif not cell.right and cell.left:
            out += close_right_face()
        elif cell.right and not cell.left: 
            out += close_left_face()
    print("Middle: " , len(out))
    return out


def pngify_row(mz: Maze, row_num: int) -> list[list[int]]:
    '''converts one row of maze cells into a chunk of png data'''
    cell_row = [boundary_row(mz, row_num)]
    
    for _ in range(ptsize - 1):
        cell_row.append(boundary_row(mz, row_num))

    #each middle row is identical to each other middle row, create one and copy.
    next_row = middle_row(mz, row_num)
    for _ in range(edgewid - (ptsize * 2)):
        cell_row.append(next_row)
    
    for _ in range(ptsize):
        cell_row.append(boundary_row(mz, row_num, top_row=False))
    
    return cell_row

def convert_to_png (mz: Maze, file):
    numrows = edgewid * mz.length()
    numcols = edgewid * mz.width

    image = Writer(width=numcols, height=numrows,
                    greyscale=True, alpha=True)
    
    image_array: list[list[int]] = pngify_row(mz, 0)

    for row in range(1, mz.width - 1):
        image_array += pngify_row(mz, row)
    image.write(file, image_array)
