#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "stddef.h"
#include "program.h"

const int data_size = 1000;

typedef struct Interpreter Interpreter;

struct Interpreter {
    Program program;
    int data[data_size];
    int counter;
};

void execute(Interpreter);
Instruction *fetch(Interpreter);

#endif
