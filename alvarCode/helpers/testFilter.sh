#!/bin/bash

# Where the testFilter.txt file is
BIN_DIR=BIN_DIR="/home/ana/Research/VisionProjectKrang/alvarCode/bin"

# Run gnuplot
"gnuplot"
plot "${BIN_DIR}/testFilter.txt" using 1:2 with lines, "${BIN_DIR}/testFilter.txt" using 1:3 with lines
