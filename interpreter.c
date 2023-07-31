#include <stdio.h>

#include "interpreter.h"
#include "program.h"

#define CHECK_DATA_BOUNDS_2(arg1, arg2) \
    arg1 < DATA_SIZE && arg2 < DATA_SIZE

#define CHECK_DATA_BOUNDS_3(arg1, arg2, arg3) \
    arg1 < DATA_SIZE && arg2 < DATA_SIZE && arg3 < DATA_SIZE

// Interpreter
void intprt_init(Interpreter *intprt, Program *program) {
    intprt->program = *program;
    intprt->counter = 0;
}

// Memory
void memory_print(Interpreter *intprt)
{
    int limit = 20;
    for (int i = 0; i < limit && i < DATA_SIZE; i++) {
        printf("[%i]:\t %i\n", i, intprt->data[i]);
    }
}

// Fetch-execute loop
void loop(Interpreter *intprt)
{
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

Instruction *fetch(Interpreter *intprt)
{
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
        case ADD:   add(intprt, dest, arg1, arg2); break;
        case ADDI:  addi(intprt, dest, arg1, arg2); break;
        case SUB:   sub(intprt, dest, arg1, arg2); break;
        case SUBI:  subi(intprt, dest, arg1, arg2); break;
        case MOV:   addi(intprt, dest, arg1, 0); break;
        case MOVI:  movi(intprt, dest, arg1); break;
        case B:     beq(intprt, dest, 0, 0); break;
        case BEQ:   beq(intprt, dest, arg1, arg2); break;
        case BEQI:  beqi(intprt, dest, arg1, arg2); break;
        case BNE:   bne(intprt, dest, arg1, arg2); break;
        case BNEI:  bnei(intprt, dest, arg1, arg2); break;
        default: printf("default\n"); break;
    }
}

// Instructions
void add(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_3(dest, arg1, arg2))
        intprt->data[dest] = intprt->data[arg1] + intprt->data[arg2];
}

void addi(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_2(dest, arg1))
        intprt->data[dest] = intprt->data[arg1] + arg2;
}

void sub(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_3(dest, arg1, arg2))
        intprt->data[dest] = intprt->data[arg1] - intprt->data[arg2];
}

void subi(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_2(dest, arg1))
        intprt->data[dest] = intprt->data[arg1] - arg2;
}

void movi(Interpreter *intprt, int dest, int arg1)
{
    if (CHECK_DATA_BOUNDS_2(dest, arg1))
        intprt->data[dest] = arg1;
}

void beq(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (dest < program_size(&intprt->program)
            && CHECK_DATA_BOUNDS_2(arg1, arg2)) {
        if (arg1 == arg2)
            intprt->counter = dest;
        else if (intprt->data[arg1] == intprt->data[arg2])
            intprt-> counter = dest;
    }
}

void beqi(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (arg1 < DATA_SIZE && dest < program_size(&intprt->program)) {
        if (intprt->data[arg1] == arg2)
            intprt->counter = dest;
    }
}

void bne(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (dest < program_size(&intprt->program)
            && CHECK_DATA_BOUNDS_2(arg1, arg2)) {
        if (intprt->data[arg1] != intprt->data[arg2])
            intprt-> counter = dest;
    }
}

void bnei(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (arg1 < DATA_SIZE && dest < program_size(&intprt->program)) {
        if (intprt->data[arg1] != arg2)
            intprt->counter = dest;
    }
}
