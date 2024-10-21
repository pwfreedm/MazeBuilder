import argparse
import os
from multiprocessing import cpu_count
from os import urandom
from time import strftime, time_ns


import maze
from src.tests.verification import check_connections
from src.outgen.pnggen import convert_to_png

#default file stuff
default_path = os.path.join(os.path.dirname(__file__), './', 'output')

def create_file (filename, filepath = default_path, extension = '.png', options = 'ab'):
    ''' creates a file given a filepath and a file name
        returns the open file object '''
    if not os.path.exists(filepath):
        os.makedirs(filepath)
    
    fullpath = os.path.join(filepath, filename + extension)
    return open(fullpath, options)


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
                    type=str,
                    default=strftime("%Y.%m.%d-%H:%M:%S"),
                    help='rename the output maze (exclude filetype)'
                    )

parser.add_argument('-n', '--nopng',
                    action='store_true',
                    default=False,
                    help='prevent pdfs of the mazes from being generated'
                    )

parser.add_argument('-fg', '--foreground',
                    action='store',
                    type=int,
                    default=0,
                    help='define the foreground color. Some helpful starting points: \
                    \n\t0  : black\n\t65 : dark grey\n\t135: light grey\n\t255: white' 
                    )

parser.add_argument('-bg', '--background',
                    action='store',
                    type=int, 
                    default=0,
                    help='define the foreground color. Some helpful starting points: \
                    \n\t0  : transparent\n\t65 : light grey\n\t135: dark grey\n\t255: black'
                    )

parser.add_argument('-e', '--edgewidth',
                    action='store',
                    type=int,
                    default=12,
                    help='define the number of pixels in each face of a given maze cell (default 12)'
                    )

parser.add_argument('-c', '--csv',
                    action='store',
                    type=str,
                    help='output a csv with maze generation data to the filename provided. This option disables png generation')

parser.add_argument('-t', '--test',
                    action='store_true',
                    default=False,
                    help='tests each maze after its generation to ensure all connections are valid. prints any errors found to stdout (default false)')

parser.add_argument('-p', '-parallel',
                    action='store_true',
                    default=False,
                    help='parallelizes maze generation with the provided algorithm')

parser.add_argument('-nc', '--num-cores',
                    action='store',
                    type=int, 
                    default=cpu_count(),
                    help='limit the number of cores used for parallelization to the provided amount. The default is the return of multiprocessing.cpu_count.')

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

parser.add_argument('-rn', '--repeatnum',
                    action='store',
                    type=int,
                    default=1,
                    help='define the number of times to repeat generation before stepping the length and width')


def main():
   args = parser.parse_args()
   start = 0
   csv = None

   for run in range(args.repeat):
      #generate blank maze
      wid = args.width + (run * args.widstep)
      len = args.length + (run * args.lenstep)

      for trial in range(args.repeatnum):

         mz = maze.Maze(len, wid)
      
         #pick and run the algorithm
         if not args.p:
            if args.algo == 'wilsons':
               start = time_ns()
               maze.Wilsons(mz, args.s)
            else:
               start = time_ns()
               maze.HK(mz, args.s)
         else:
            start = time_ns()
            mz = maze.parallelize(args.algo, args.length, args.width, args.s, args.num_cores)

         
         #calculate time for csv later
         runtime = time_ns() - start

         #optionally verify connections
         if args.test: 
            check_connections(mz, args.s, silent=False)
         
         #update seed before next run if needed
         if not args.keepseed:
            args.s = int.from_bytes(urandom(4), signed=True)

         #generate the output png - skip pngs if writing CSV to save time
         if not args.nopng and not args.csv:
            file = create_file(args.output)
            convert_to_png(mz, file, args.edgewidth, args.foreground, args.background)
            file.close()
         
         if args.csv: 
            if run == 0 and trial == 0:
               filename = str(args.algo).title() + '-' + strftime("%d-%H:%M:%S")
               csv = create_file(filename, extension='.csv', options='w+')
               csv.write('Seed,Length,Width,Time(ns),Passed Verification\n')
            csv.write(str(str(args.s) + ',' + str(len) + ',' + str(wid) + ',' + str(runtime) + ',' + str(check_connections(mz, args.s)) + '\n'))
   #if a csv was created, close it after mazes are tested
   if csv:
      csv.close()
            
         
if __name__ == '__main__':
   main()