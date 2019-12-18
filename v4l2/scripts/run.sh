#!/bin/bash
set -e
reset

clang-format -i webcam.c
gcc webcam.c -o webcam
./webcam
# python plot_image.py

# clang-format -i capture2.c
# gcc capture2.c -o capture2
# sudo ./capture2
