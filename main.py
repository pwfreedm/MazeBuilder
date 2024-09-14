import argparse

'''supportable options: 
        [-h, help]; displays all commands
        [-a, --algo] N; selects an algorithm to use (default Wilsons)
        [-w, --width] N; defines the width of the maze to generate
        [-l, --length] N; defines the length of the maze to generate
        [-r, --repeat]; defines the number of times to repeat the process
        [-ls, --len-step] N; defines the step size for the length between trials
        [-ws, --wid-step] N; defines the step size for the width between trials
        [-o, --output] N; defines a filepath to write data to
        [-t, --test]; runs mazes through the connection verification algorithm
        [-v, --verbose]; runs mazes through the connection verification algorithm and prints verbose output
        [-d, --debug] N; defines an output destination for debug information (default stdout)

'''
parser = argparse.ArgumentParser(description="CLI Parser for Maze Generation")
parser.add_argument()