#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

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
    BLEI,
    RET,
    RETI,
    HALT
} OpCode;

// Instruction struct
//
// +----------+----------+
// | code(16) | dest(16) |
// +----------+----------+
// |       arg1(32)      |
// +---------------------+
// |       arg2(32)      |
// +---------------------+

typedef struct {
    uint16_t code; // OpCode enum
    uint16_t dest;
    int32_t arg1;
    int32_t arg2;
} Instruction;

typedef struct {
    Instruction *v;
    size_t capacity; // maximum size before re-init
    size_t size; // current size
} Program;

bool program_init(Program *program);
bool program_deinit(Program *program);
size_t program_size(Program *program);
bool program_resize(Program *program, size_t capacity_new);
bool program_inc_capacity(Program *program);
bool program_merge(Program *program1, Program *program2);
bool program_add(Program *program, Instruction inst);
void program_print(Program *program);
void inst_print(Instruction inst, size_t index);
Instruction *program_fetch(Program *program, size_t index);

#endif
