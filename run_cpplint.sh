#!/bin/bash

# A bash script to run cpplint on all header and source files

# Stop on error
set -e

FILES=$(find include src -type f \( -name "*.h" -o -name "*.cpp" \))

python cpplint.py $FILES