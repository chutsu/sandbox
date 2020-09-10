#!/bin/bash
set -e

gcc -O0 array_test.c -o bin/array_test_c
g++ -O0 array_test.cpp -o bin/array_test_cpp
g++ -O0 array_vector_test.cpp -o bin/array_vector_test_cpp

time ./bin/array_vector_test_cpp
time ./bin/array_test_c
time ./bin/array_test_cpp
