#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdbool.h>

typedef enum {
    ADD,
    ADDI,
    SUB,
    SUBI,
    MOV,
    MOVI,
    B,
    BEQ,
    BEQI,
    BNE,
    BNEI
} op;

typedef struct {
    op code;
    unsigned int dest;
    int arg1;
    int arg2;
} Instruction;

typedef struct {
    Instruction *v;
    size_t capacity; // maximum size before re-init
    size_t size; // current size
} Program;

bool program_init(Program *program);
bool program_deinit(Program *program);
size_t program_size(Program *program);
bool program_inc_capacity(Program *program);
bool program_add(Program *program, Instruction inst);
void program_print(Program *program); // program, index
void inst_print(Instruction inst, size_t index);
Instruction *program_fetch(Program *program, size_t index); // inst, index

#endif
