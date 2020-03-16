#!/bin/bash
set -e

mkdir -p bin
gcc -o bin/time_dgemm time_dgemm.c -lopenblas
./bin/time_dgemm 1000 1000 1000
