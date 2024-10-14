from png import Writer

from maze import Maze

class metadata: 
    '''contains metadata used in generating png
    
    fields: 
        edgewid: the number of pixes that each cell edge should be
        ptsize: the thickness of the lines in the maze (DO NOT CHANGE)
        fg: the color of the foreground pixel (0 = transparent, 255 = black)
        bg: the color of the background pixel (0 = transparent, 255 = black)
    '''
    def __init__ (self, edgewid: int, fg: int, bg: int):
        self.edgewid = edgewid
        self.ptsize = 1
        self.fg_px = [fg, 255]
        self.bg_px = [bg, 0 if bg == 0 else 255]

#=============================================================================#
#helper functions so actual code reads more clearly

def close_top_face (md: metadata) -> list[int]:
    return md.fg_px * md.edgewid

def close_bottom_face(md: metadata) -> list[int]:
    return md.fg_px * md.edgewid

def close_left_face(md: metadata) -> list[int]:
    return (md.fg_px * md.ptsize) + md.bg_px * (md.edgewid - md.ptsize)

def close_right_face(md: metadata) -> list[int]:
    return (md.bg_px * (md.edgewid - md.ptsize)) + md.fg_px * md.ptsize

def close_lr_face(md: metadata) -> list[int]:
    return md.fg_px * md.ptsize + md.bg_px * (md.edgewid - 2 * md.ptsize) + md.fg_px * md.ptsize

def open_face(md: metadata) -> list[int]:
    return md.bg_px * md.edgewid


#=============================================================================#

def boundary_row(md: metadata, mz: Maze, row_num: int, top_row: bool = True) -> list[int]:
    out = []

    for idx in range(mz.width):
        side = mz.getSide(idx)
        cell = mz[(mz.width * row_num) + idx]
        if top_row and not cell.up(side): 
            out += close_top_face(md)
        elif not top_row and not cell.down(side): 
            out += close_bottom_face(md)
        else:
            out += close_lr_face(md)
    
    return out

def middle_row(md: metadata, mz: Maze, row_num: int) -> list[int]:
    out = []

    for idx in range(mz.width):
        side = mz.getSide(idx)
        cell = mz[(mz.width * row_num) + idx]
        if not cell.right(side) and not cell.left(side): 
            out += close_lr_face(md)
        elif not cell.right(side) and cell.left(side):
            out += close_right_face(md)
        elif cell.right(side) and not cell.left(side): 
            out += close_left_face(md)
        else:
            out += open_face(md)

    return out


def pngify_row(md: metadata, mz: Maze, row_num: int) -> list[list[int]]:
    '''converts one row of maze cells into a chunk of png data'''
    cell_row = [boundary_row(md, mz, row_num)]
    
    for _ in range(md.ptsize - 1):
        cell_row.append(boundary_row(md, mz, row_num))

    #each middle row is identical to each other middle row, create one and copy.
    next_row = middle_row(md, mz, row_num)
    for _ in range(md.edgewid - (md.ptsize * 2)):
        cell_row.append(next_row)
    
    for _ in range(md.ptsize):
        cell_row.append(boundary_row(md, mz, row_num, top_row=False))
    
    return cell_row

def convert_to_png (mz: Maze, file, edgewid: int = 12, fg: int = 255, bg: int = 0):
    
    md = metadata(edgewid, fg, bg)

    numrows = md.edgewid * mz.length()
    numcols = md.edgewid * mz.width

    image = Writer(width=numcols, height=numrows,
                    greyscale=True, alpha=True)
    
    image_array: list[list[int]] = pngify_row(md, mz, 0)

    for row in range(1, mz.width):
        image_array += pngify_row(md, mz, row)
    image.write(file, image_array)
