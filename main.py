import argparse
from os import urandom
from pathlib import Path


from maze import Maze, Wilsons, HK
from src.tests.verification import check_connections

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
        [-d, --debug] N; defines an output destination for debug information
        (default stdout)
        [-rs, --regenseed]; regenerates the seed between repeated runs

'''
parser = argparse.ArgumentParser(prog="MazeBuilder",
                                 description="CLI Parser for Maze Generation")

parser.add_argument('-a', '--algo',
                    action='store',
                    choices=['wilsons', 'hk'],
                    default='wilsons',
                    help="select which algorithm should be used to generate the output maze (default wilsons)")

parser.add_argument('-s', '-seed',
                    action='store',
                    type=int,
                    default=int.from_bytes(urandom(4), signed=True),
                    help='generates the maze with seed S (default random)')

parser.add_argument('-ks', '--keepseed',
                    action='store_true',
                    default=False,
                    help='if this flag is provided, the same seed will be used for each run in repeat trials')

parser.add_argument('-w', '--width',
                    action='store',
                    type=int,
                    default=50,
                    help="define the width of the maze (default 50)")

parser.add_argument('-l', '--length',
                    action='store',
                    type=int,
                    default=50,
                    help='define the length of the maze (default 50)')

#TODO: this will not work on Windows because ./output is an invalid path
parser.add_argument('-o', '--output',
                    action='store',
                    type=Path,
                    default=Path('./output'),
                    help='define the directory to which output should be stored'
                    )

parser.add_argument('-n', '--nopdf',
                    action='store_true',
                    default=False,
                    help='prevent pdfs of the mazes from being generated')

parser.add_argument('-c', '--csv',
                    action='store_true',
                    default=False,
                    help='output a csv with maze generation data')

parser.add_argument('-t', '--test',
                    action='store_true',
                    default=False,
                    help='tests each maze after its generation to ensure all connections are valid. if used in conjunction with --csv, adds that data to the csv (default false)')

parser.add_argument('-v', '--verbose',
                    action='store_true',
                    default=False,
                    help='tests each maze after its generation, returning all connections in the maze that are invalid. If used with --csv, adds this data to the csv (default false)')

parser.add_argument('-d', '--debug',
                    action='store_true',
                    default=False,
                    help='print debug information (this may be nothing, who knows.) (default false)')

parser.add_argument('-r', '--repeat',
                    action='store',
                    type=int, 
                    default=1,
                    help='define the number of times to repeat maze generation (default 1)')

parser.add_argument('-ls', '--lenstep',
                    action='store',
                    type=int,
                    default=0,
                    help='define the amount to grow the length of the maze by between repetitions. Ignored if repetitions = 0 (default 0)')

parser.add_argument('-ws', '--widstep',
                    action='store',
                    type=int,
                    default=0,
                    help='define the amount to grow the width of the maze by between repetitions. Ignored if repetitions = 0 (default 0)')


def main():
   args = parser.parse_args()
   for run in range(args.repeat):
      #generate blank maze
      wid = args.width + (run * args.widstep)
      len = args.length + (run * args.lenstep)
      mz = Maze(len, wid)

      #pick and run the algorithm
      if args.algo == 'wilsons':
         Wilsons(mz, args.s)
      else:
         HK(mz, args.s)
      
      #optionally verify connections
      if args.test and not args.verbose:
         check_connections(mz, args.s)
      if args.verbose: 
         check_connections(mz, args.s, False)
      
      #update seed before next run if needed
      if not args.keepseed:
         args.s = int.from_bytes(urandom(4), signed=True)



if __name__ == '__main__':
   main()