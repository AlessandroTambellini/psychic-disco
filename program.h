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

typedef struct {
    uint32_t code; // enum OpCode
    uint32_t dest;
    uint32_t arg1;
    uint32_t arg2;
} Instruction;

typedef struct {
    Instruction *v;
    size_t capacity; // maximum size before re-init
    size_t size; // current size
} Program;

bool program_init(Program *program);
bool program_deinit(Program *program);
Instruction *program_data(Program *program);
size_t program_capacity(Program *program);
size_t program_size(Program *program);
bool program_resize(Program *program, size_t capacity_new);
bool program_inc_capacity(Program *program);
bool program_clear(Program *program);
bool program_copy(Program *program, Program *src);
bool program_merge(Program *program, Instruction *src, size_t size);
bool program_split(Program *program, Instruction *dst, size_t size);
bool program_delete(Program *program, size_t start, size_t size);
bool program_get(Program *program, Instruction *dst, size_t start, size_t size);
bool program_add(Program *program, Instruction inst);
void program_print(Program *program);
void inst_print(Instruction inst, size_t index);
Instruction *program_fetch(Program *program, size_t index);

#endif
