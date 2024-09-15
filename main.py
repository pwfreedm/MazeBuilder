import argparse
from pathlib import Path
'''supportable options: 
        [-h, help]; displays all commands
        [-a, --algo] N; selects an algorithm to use (default Wilsons)
        [-s, --seed] N; allows the selection of a seed (default random)
        [-w, --width] N; defines the width of the maze to generate
        [-l, --length] N; defines the length of the maze to generate
        [-n, --no-pdf]; disables pdf generation (default False)
        [-c, --csv]; exports timing data as a csv (also enables timing) (default True)
        [-r, --repeat]; defines the number of times to repeat the process
        [-ls, --len-step] N; defines the step size for the length between trials
        [-ws, --wid-step] N; defines the step size for the width between trials
        [-o, --output] N; defines a filepath to write data to
        [-t, --test]; runs mazes through the connection verification algorithm
        [-v, --verbose]; runs mazes through the connection verification algorithm and prints verbose output
        [-d, --debug] N; defines an output destination for debug information (default stdout)

'''
parser = argparse.ArgumentParser(prog="MazeBuilder",
                                 description="CLI Parser for Maze Generation")
subparsers = parser.add_subparsers(help='help with the subcommands to the --repeat command')

parser.add_argument('-a', '--algo',
                    action='store',
                    choices=['wilsons', 'hk'],
                    default='wilsons',
                    nargs=2,
                    help="select which algorithm(s) should be used to generate the output maze(s) (default wilsons)")

parser.add_argument('-s', '-seed',
                    action='store',
                    type=int,
                    default=0,
                    nargs=1,
                    help='generates the maze with seed S (default 0)')

parser.add_argument('-w', '--width',
                    action='store',
                    type=int,
                    default=50,
                    nargs=1,
                    help="define the width of the maze (default 50)")

parser.add_argument('-l', '--length',
                    action='store',
                    type=int,
                    default=50,
                    nargs=1,
                    help='define the length of the maze (default 50)')

#TODO: this will not work on Windows because ./output is an invalid path
parser.add_argument('-o', '--output',
                    action='store',
                    type=Path,
                    default=Path('./output'),
                    nargs=1,
                    help='define the directory to which output should be stored'
                    )

parser.add_argument('-n', '--no-pdf',
                    action='store_true',
                    default=False,
                    nargs=1,
                    help='prevent pdfs of the mazes from being generated')

parser.add_argument('-c', '--csv',
                    action='store_true',
                    default=False,
                    nargs=1,
                    help='output a csv with maze generation data')

parser.add_argument('-t', '--test',
                    action='store_true',
                    default=False,
                    nargs=1,
                    help='tests each maze after its generation to ensure all connections are valid. if used in conjunction with --csv, adds that data to the csv (default false)')

parser.add_argument('-v', '--verbose',
                    action='store_true',
                    default=False,
                    nargs=1,
                    help='tests each maze after its generation, returning all connections in the maze that are invalid. If used with --csv, adds this data to the csv (default false)')

parser.add_argument('-d', '--debug',
                    action='store_true',
                    default=False,
                    nargs=1,
                    help='print debug information (this may be nothing, who knows.) (default false)')

parser.add_argument('-r', '--repeat',
                    action='store',
                    type=int, 
                    default=0,
                    nargs=1,
                    help='define the number of times to repeat maze generation (default 0)')

repeat_subparser = subparsers.add_parser('repeat steps',
                                         help='command definitions for subcommands to the repeat command')

repeat_subparser.add_argument('-ls', '--len-step',
                              action='store',
                              type=int,
                              default=0,
                              nargs=1,
                              help='define the amount to grow the length of the maze by between repetitions (default 0)')

repeat_subparser.add_argument('-ws', '--wid-step',
                              action='store',
                              type=int,
                              default=0,
                              nargs=1,
                              help='define the amount to grow the width of the maze by between repetitions (default 0)')

#TODO: expand this to a proper main of some kind
args = parser.parse_args()
print(args.algo)