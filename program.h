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
    PUSH,
    PUSHI,
    POP,
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

const static char *opcode_of[] = {
    [ADD]   = "add",
    [ADDI]  = "addi",
    [SUB]   = "sub",
    [SUBI]  = "subi",
    [MUL]   = "mul",
    [MULI]  = "muli",
    [DIV]   = "div",
    [DIVI]  = "divi",
    [MOV]   = "mov",
    [MOVI]  = "movi",
    [PUSH]  = "push",
    [PUSHI] = "pushi",
    [POP]   = "pop",
    [B]     = "b",
    [BEQ]   = "beq",
    [BEQI]  = "beqi",
    [BNE]   = "bne",
    [BNEI]  = "bnei",
    [BGE]   = "bge",
    [BGEI]  = "bgei",
    [BLEI]  = "blei",
    [RET]   = "ret",
    [RETI]  = "reti",
    [HALT]  = "halt",
};

// This is the supposed maximum length of a
// string containing an opcode
#define OPCODE_SIZE 16

typedef struct {
    uint32_t code; // enum OpCode
    uint32_t dest;
    uint32_t arg1;
    uint32_t arg2;
} Instruction;

// This is the supposed maximum length of a
// string containing an instruction
#define INST_SIZE 128

typedef struct {
    Instruction *items;
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
size_t program_delete(Program *program, size_t start, size_t size);
size_t program_get(Program *program, Instruction *dst, size_t start, size_t size);
bool program_add(Program *program, Instruction inst);
Instruction *program_fetch(Program *program, size_t index);
bool program_save(char *filename, Program *program);
bool program_load(char *filename, Program *program);
void program_print(Program *program);
void inst_print(Instruction inst, size_t index);
bool opcode_decode(char *buffer, OpCode *code);
bool inst_decode(Instruction *inst, char *buffer);
bool inst_encode(char *buffer, Instruction *inst);

#endif
