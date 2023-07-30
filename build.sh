#!/bin/bash

set -xe 

gcc main.c program.c interpreter.c stack.c -o psychic-disco
