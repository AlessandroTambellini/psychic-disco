#include <stdio.h>

#include "interpreter.h"
#include "program.h"

// Memory
void memory_print(Interpreter *intprt) {
    int limit = 20;
    for (int i = 0; i < limit && i < DATA_SIZE; i++) {
        printf("[%i]: %i\n", i, intprt->data[i]);
    }
}

// Fetch-execute loop
void loop(Interpreter *intprt) {
    Instruction *inst;

    while (intprt->counter < intprt->program.size) {
        // Fetch instruction
        inst = fetch(intprt);

        // Increment program counter
        intprt->counter++;

        // Execute instruction
        execute(intprt, inst);
    }
}

Instruction *fetch(Interpreter *intprt) {
    size_t counter = intprt->counter;
    return program_fetch(&intprt->program, counter);
}

// Pointer to interpreter cause we need
// to increase the program counter
void execute(Interpreter *intprt, Instruction *inst)
{
    int dest = inst->dest;
    int arg1 = inst->arg1;
    int arg2 = inst->arg2;
    switch (inst->code) {
        case 0: add(intprt, dest, arg1, arg2); break;
        case 1: addi(intprt, dest, arg1, arg2); break;
        case 2: sub(intprt, dest, arg1, arg2); break;
        case 3: subi(intprt, dest, arg1, arg2); break;
        case 4: printf("Instruction 4 not yet implemented.\n"); break;
        case 5: printf("Instruction 5 not yet implemented.\n"); break;
        default: printf("default\n"); break;
    }
}

// Instructions
void add(Interpreter *intprt, int dest, int arg1, int arg2) {
    intprt->data[dest] = intprt->data[arg1] + intprt->data[arg2];
}

void addi(Interpreter *intprt, int dest, int arg1, int arg2) {
    intprt->data[dest] = intprt->data[arg1] + arg2;
}

void sub(Interpreter *intprt, int dest, int arg1, int arg2) {
    intprt->data[dest] = intprt->data[arg1] - intprt->data[arg2];
}

void subi(Interpreter *intprt, int dest, int arg1, int arg2) {
    intprt->data[dest] = intprt->data[arg1] - arg2;
}
