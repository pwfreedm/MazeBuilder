#!/bin/bash

python mazebuilder.py -a hk -s 0 -w 64 -l 64 -c -o Parallel_HK -p -r 100 -ls 64 -ws 64 -rn 25
python mazebuilder.py -a wilsons -s 0 -w 64 -l 64 -c -o Parallel_Wilsons -p -r 100 -ls 64 -ws 64 -rn 25
python mazebuilder.py -a hk -s 0 -w 64 -l 64 -c -o Serial_HK -r 100 -ls 64 -ws 64 -rn 25
python mazebuilder.py -a wilsons -s 0 -w 64 -l 64 -c -o Serial_Wilsons -r 100 -ls 64 -ws 64 -rn 25