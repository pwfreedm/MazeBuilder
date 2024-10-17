# Maze Builder
This program is designed to allow you to build (reasonably) memory and time efficient mazes. 
It is a CLI tool that can produce pngs of mazes with arbitrary greyscale foreground and 
background colors. The cli is built in python but uses a maze package written in C++, for efficiency. 
There are two algorithms, Wilson's Algorithm and the Hunt and Kill algorithm; the former tends to make 
mazes with shorter corridors and more turns where the latter tends to favor longer straight segments.

[My publication outlining project intent](http://granite.sru.edu/~pacise/pacise2024/ParallelMazeGeneration.pdf)

Note that a lot of the details in this publication are no longer accurate. A follow-up paper will be submitted to PACISE 2025 with updated details. Almost all discussion about memory is now irrelevant, as it has all been optimized. 

# Setup
All setup instructions will be provided assuming a Linux environment. If you are on Mac or Windows, it should be easy enough to translate them.

<B> This project requires Python 3.11 or higher and CMake 3.29.2 or higher to run </B>

<ol>
<li> Clone the repo
<li> Navigate to the top level directory (should be ./MazeBuilder)
<li> If you aren't already in a virtual environment, create one and enter it.
<li> Type "pip install -e ." and wait until it says "maze ... successfully installed"
<li> Type "python mazebuilder.py" to generate a maze using the default configuration
</ol>



# Usage
This section will be split into two sections: basic usage and profiling. This is because a number of the flags available only really matter if you want to profile this program or your CPU. 

<B> The default configuration generates a 50 x 50 maze with Wilsons algorithm serially and outputs it as a black maze with a transparent background to a file named with the date and time of generation to MazeBuilder/output. </B>


## Basic Usage Commands
`` -h, --help `` - displays a help menu that lists all of this information <br>
`` -a, --algo [wilsons, hk] `` - allows manual selection of the algorithm used for maze generation (default is wilsons) <br>
`` -s, --seed N `` - allows seeding the random number generator. The same seed will guarantee the same maze on successive invocations given that the algorithm used is the same as well.\ (default is a random signed 4 byte int generated with urandom) <br>
`` -w, --width N`` - allows definition of the width of the generated maze (default 50) <br>
`` -l, --length N`` - allows definition of the length of the generated maze (default 50) <br>
`` -o, --output `` - allows renaming of the output pdf (default is the date and time of generation) <br>
`` -n, --nopng `` - skips png generation (default false) <br>
`` -fg, --foreground`` - allows definition of the color of the maze walls. Some helpful starting values: black = 0, dark grey = 65, light grey = 135, white = 255 (default 0) <br>
`` -bg, --background`` - allows definition of the color of the background of the image. Some helpful starting values: 0 = transparent, 65 = light grey, 135 = dark grey, 255 = black (default 0) <br>
``-e, --edgewidth`` - defines the number of pixels on each edge of each cell in the maze. This means that the generated png will be width * edgewidth x length * edgewidth. BE CAREFUL CHANGING THIS - values too small will result in a solid image and values too large will make the walls hard to see, as they do not grow in thickness proportionally to the edge width (default 12) <br>


## Profiling Commands
`` -r, --repeat N`` - generate a maze N times (default 1) <br>
`` -ls, --lenstep N`` - amount to increase the length of the maze by between repetitions (default 0) <br>
`` -ws, --widstep N`` - amount to increase the width of the maze by between repetitions (default 0) <br>
`` -rn, --repeatnum N`` -  number of mazes to generate before stepping up length and width (default 1) <br>
``-c, --csv filename`` - export maze data (seed, size, time, valid generation) to csv file filename. This option automatically disables png generation (default filename is algorithm-time.csv) <br>
``-p, --parallel`` - parallelize maze generation (default false) <br>
``-nc, --num-cores N`` - override to select the number of cores used for parallelization (default is the result of multiprocessing::cpu_count()) <br>
``-t, --test`` - test each maze after generation to insure it can be solved. This flag is automatically enabled if a csv is being generated. This flag prints every error found with a given maze to stdout (default false) <br>

# Learn about the algorithms

[Original Wilsons Paper](https://www.cs.cmu.edu/~15859n/RelatedWork/RandomTrees-Wilson.pdf) <br>
[Jamis Buck's blog post about Wilsons](https://weblog.jamisbuck.org/2011/1/20/maze-generation-wilson-s-algorithm) <br>
[Jamis Buck's blog post about Hunt and Kill](https://weblog.jamisbuck.org/2011/1/24/maze-generation-hunt-and-kill-algorithm) <br>

# Acknowledgements

Thank you to Dr. Zoppetti, my advisor, for all his help (and occasionally telling me to get it together). His encouragement made the worst hills on this journey easier to climb. <br>

Thank you to Jamis Buck. He has no clue who I am, but his website has very well decomposed explanations of how different maze generation algorithms work. His explanations of these are more clear than mine could be, so his links are above. <br>

Thank you to Will Killian. He has sat in on a number of my Thesis progress meetings and always had incredibly helpful and insightful additions. His contributions pushed me down paths I wouldn't have walked otherwise, and I learned a lot in the process of implementing those suggestions. <br>