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
    int code; // enum OpCode
    int dest;
    int arg1;
    int arg2;
} Instruction;

typedef struct {
    Instruction *v;
    size_t capacity; // maximum size before re-init
    size_t size; // current size
} Program;

typedef enum {
    MERGE,
    EXEC,
    RESET,
} MsgType;

#define PAYLOAD_SIZE 4096

typedef struct {
    int32_t type;
    uint32_t size;
    Instruction v[PAYLOAD_SIZE];
} Msg;

typedef struct {
    int ret;
} ResultMsg;

bool program_init(Program *program);
bool program_deinit(Program *program);
size_t program_size(Program *program);
bool program_resize(Program *program, size_t capacity_new);
bool program_inc_capacity(Program *program);
bool program_clear(Program *program);
bool program_merge(Program *program1, Program *program2);
bool program_merge_msg(Program *program, Msg *msg);
bool program_add(Program *program, Instruction inst);
void program_print(Program *program);
void inst_print(Instruction inst, size_t index);
Instruction *program_fetch(Program *program, size_t index);

#endif
