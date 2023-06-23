#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "stddef.h"
#include "program.h"

#define DATA_SIZE 1000

typedef struct Interpreter Interpreter;

struct Interpreter {
    Program program;
    int counter;
    int data[DATA_SIZE];
};

// Memory
void memory_print(Interpreter *);

// Fetch-execute loop
void loop(Interpreter *);
Instruction *fetch(Interpreter *);
void execute(Interpreter *, Instruction *);

// Instruction body
void add(Interpreter *intprt, int dest, int arg1, int arg2);
void addi(Interpreter *intprt, int dest, int arg1, int arg2);
void sub(Interpreter *intprt, int dest, int arg1, int arg2);
void subi(Interpreter *intprt, int dest, int arg1, int arg2);

#endif
