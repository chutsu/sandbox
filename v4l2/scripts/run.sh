#!/bin/bash
set -e
reset

mkdir -p bin
clang-format -i webcam.c
gcc webcam.c -o bin/webcam
./bin/webcam
# python plot_image.py

# clang-format -i capture2.c
# gcc capture2.c -o capture2
# sudo ./capture2
