#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdbool.h>
#include <stdio.h>

typedef enum {
    ADD,
    ADDI,
    SUB,
    SUBI,
    MUL,
    MULI,
    DIV,
    DIVI,
    MOV,
    MOVI,
    B,
    BEQ,
    BEQI,
    BNE,
    BNEI,
    BGE,
    BGEI,
    RET,
    RETI,
    HALT
} OpCode;

typedef struct {
    OpCode code;
    unsigned dest;
    int arg1;
    int arg2;
} Instruction;

typedef struct {
    Instruction* v;
    size_t capacity; // maximum size before re-init
    size_t size; // current size
} Program;

bool program_init(Program* program);
bool program_deinit(Program* program);
size_t program_size(Program* program);
bool program_inc_capacity(Program* program);
bool program_add(Program* program, Instruction inst);
void program_print(Program* program);
void inst_print(Instruction inst, size_t index);
Instruction* program_fetch(Program* program, size_t index);

#endif
