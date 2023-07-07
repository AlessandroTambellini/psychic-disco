#!/bin/sh

set -xe

clang program.c interpreter.c main.c -o psychic-disco
