#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "stddef.h"
#include "program.h"

// modify DATA_SIZE to be a const int
#define DATA_SIZE 1000

typedef enum {
    OK,
    DATA_OVERFLOW,
    MALFORMED_INSTRUCTION,
    DIVISION_BY_ZERO
} InstResult;

#define RES_STRING(res) #res
static const char *res_names[] = {
    RES_STRING(OK),
    RES_STRING(DATA_OVERFLOW),
    RES_STRING(MALFORMED_INSTRUCTION),
    RES_STRING(DIVISION_BY_ZER)
};
#undef RES_STRING

typedef struct {
    Program program;
    int pc;
    int data[DATA_SIZE];
} Vm;

// Interpreter
void vm_init(Vm *vm, Program *program);

// Memory
void memory_print(Vm *vm);

// Fetch-execute loop
void loop(Vm *vm);
Instruction *fetch(Vm *vm);
InstResult execute(Vm *vm, Instruction *inst);

// Instruction body
InstResult add(Vm *vm, int dest, int arg1, int arg2);
InstResult addi(Vm *vm, int dest, int arg1, int arg2);
InstResult sub(Vm *vm, int dest, int arg1, int arg2);
InstResult subi(Vm *vm, int dest, int arg1, int arg2);
InstResult subi(Vm *vm, int dest, int arg1, int arg2);
InstResult mul(Vm *vm, int dest, int arg1, int arg2);
InstResult muli(Vm *vm, int dest, int arg1, int arg2);
InstResult div(Vm *vm, int dest, int arg1, int arg2);
InstResult divi(Vm *vm, int dest, int arg1, int arg2);
InstResult movi(Vm *vm, int dest, int arg1);
InstResult beq(Vm *vm, int dest, int arg1, int arg2);
InstResult beqi(Vm *vm, int dest, int arg1, int arg2);
InstResult bne(Vm *vm, int dest, int arg1, int arg2);
InstResult bnei(Vm *vm, int dest, int arg1, int arg2);
InstResult blei(Vm *vm, int dest, int arg1, int arg2);
InstResult bgei(Vm *vm, int dest, int arg1, int arg2);
InstResult ret(Vm *vm, int dest);
InstResult reti(Vm *vm, int dest);
InstResult halt(Vm *vm);

#endif
