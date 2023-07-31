#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "stddef.h"
#include "program.h"

#define DATA_SIZE 1000

typedef enum {
    OK,
    OVERFLOW
} InstResult;

typedef struct {
    Program program;
    int counter;
    int data[DATA_SIZE];
} Interpreter;

// Interpreter
void intprt_init(Interpreter *intprt, Program *program);

// Memory
void memory_print(Interpreter *);

// Fetch-execute loop
void loop(Interpreter *);
Instruction *fetch(Interpreter *);
InstResult execute(Interpreter *, Instruction *);

// Instruction body
InstResult add(Interpreter *intprt, int dest, int arg1, int arg2);
InstResult addi(Interpreter *intprt, int dest, int arg1, int arg2);
InstResult sub(Interpreter *intprt, int dest, int arg1, int arg2);
InstResult subi(Interpreter *intprt, int dest, int arg1, int arg2);
InstResult movi(Interpreter *intprt, int dest, int arg1);
InstResult beq(Interpreter *intprt, int dest, int arg1, int arg2);
InstResult beqi(Interpreter *intprt, int dest, int arg1, int arg2);
InstResult bne(Interpreter *intprt, int dest, int arg1, int arg2);
InstResult bnei(Interpreter *intprt, int dest, int arg1, int arg2);

#endif
