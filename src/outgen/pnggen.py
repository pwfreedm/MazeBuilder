import png

#png is encoded in greyscale-alpha, so pixels only have two specifiers
bl = [0,0]
tr = [0,255]

'''General Approach: 
    each line in the output maze will be 3 pixels thick
    each cell will be 18 pixels thick (minus 3 for its top line)

    0) create file and necessary objects
    1) insert the top row of the maze
    2) pull in the first full row of cells
    3) iterate through in 3 pixel chunks
    4) find some smart way to figure out whats black
    5) profit???
'''

