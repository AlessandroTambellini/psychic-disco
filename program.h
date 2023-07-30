#ifndef ACTIONS_H
#define ACTIONS_H

// typedef enum op {
//     ADD,
//     ADDI,
//     SUB,
//     SUBI,
//     MOV,
//     MOVI,
//     B,
//     BEQ,
//     BEQI,
//     BNE,
//     BNEI
// };

typedef struct {
    // op code;
    unsigned int code;
    unsigned int dest;
    unsigned int arg1;
    unsigned int arg2;
} Instruction;

typedef struct {
    Instruction *v;
    size_t capacity; // maximum size before re-init
    size_t size; // current Actions size
} Program;

int program_init(Program *);
int program_deinit(Program *);
size_t program_size(Program *);
int program_inc_capacity(Program *);
int program_add(Program *, Instruction);
Instruction *program_fetch(Program *, size_t); // program, index
void program_print(Program *);
void inst_print(Instruction, size_t); // inst, index

#endif
