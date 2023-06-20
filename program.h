#ifndef ACTIONS_H
#define ACTIONS_H

typedef struct Instruction Instruction;

struct Instruction {
    int code;
    int dest;
    int arg1;
    int arg2;
};

typedef struct Program Program;

struct Program {
    Instruction *v;
    size_t capacity; // maximum size before re-init
    size_t size; // current Actions size
};

int program_init(Program *);
int program_deinit(Program *);
size_t program_size(Program *);
int program_inc_capacity(Program *);
int program_add(Program *, Instruction);
Instruction *program_fetch(Program *, size_t); // program, index
void program_print(Program *);
void inst_print(Instruction, size_t); // inst, index

#endif
