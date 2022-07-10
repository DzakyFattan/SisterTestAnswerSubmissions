#!/bin/bash

export OP_NUM_THREADS=4
gcc -fopenmp grep.c filehandler.c filenode.c -o grep