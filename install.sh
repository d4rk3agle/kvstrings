#!/usr/bin/bash
gcc -O3 ./src/*.c -o kvstrings
mv ./kvstrings /usr/local/bin
rm -rf ./kvstrings