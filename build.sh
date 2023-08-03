#!/bin/bash

set -xe 

# Check if clang is avaiable
if command -v clang >/dev/null 2>&1; then
    # Compile with clang
    clang main.c program.c interpreter.c -o psychic-disco
# Otherwise check if gcc is avaiable
elif command -v gcc >/dev/null 2>&1; then
    # Compile with gcc
    gcc main.c program.c interpreter.c -o psychic-disco
else 
    echo "Error: Neither clang nor gcc is avaiable."
    exit 1
fi