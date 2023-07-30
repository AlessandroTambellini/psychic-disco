#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdbool.h>

enum op {
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
};

typedef struct {
    enum op code;
    unsigned int dest;
    int arg1;
    int arg2;
} Instruction;

typedef struct {
    Instruction *v;
    size_t capacity; // maximum size before re-init
    size_t size; // current size
} Program;

bool program_init(Program *);
bool program_deinit(Program *);
size_t program_size(Program *);
bool program_inc_capacity(Program *);
bool program_add(Program *, Instruction);
Instruction *program_fetch(Program *, size_t); // program, index
void program_print(Program *);
void inst_print(Instruction, size_t); // inst, index

#endif
