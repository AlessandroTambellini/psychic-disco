#!/bin/bash

set -xe 

# Check if Clang is avaiable
if command -v clang >/dev/null 2>&1; then
    # Compile with Clang
    clang main.c program.c interpreter.c -o psychic-disco
# Otherwise check if G++ is avaiable
elif command -v g++ >/dev/null 2>&1; then
    # Compile with g++
    g++ main.c program.c interpreter.c -o psychic-disco
else 
    echo "Error: Neither Clang nor g++ is avaiable."
    exit 1
fi