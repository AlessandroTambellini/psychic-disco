#include <stdlib.h>
#include <stdio.h>
#include "program.h"

int program_init(Program *program) {
    program->v= (Instruction *) malloc(sizeof(Instruction));
    if (program->v == NULL)
        return 0;

    program->capacity = 1;
    program->size = 0;

    return 1;
}

int program_deinit(Program *program) {
    if (program == NULL)
        return 0;

    if (program->v == NULL) {
        return 0;
    }
    else {
        free(program->v);
    }

    free(program);

    return 1;
}

size_t program_size(Program *program) {
    return program->size;
}

int program_inc_capacity(Program *program) {
    Instruction *v_old = program->v;

    size_t capacity_new = 2 * program->capacity;

    program->v = (Instruction *) malloc(capacity_new * sizeof(Instruction));
    if (program->v == NULL)
        return 0;

    for (size_t i = 0; i < program->size; i++) {
        program->v[i] = v_old[i];
    }

    free(v_old);

    program->capacity = capacity_new;

    return 1;
}

int program_add(Program *program, Instruction inst) {
    if (program->size == program->capacity) {
        if (!program_inc_capacity(program)) {
            printf("Failed to increase capacity.\n");
            return 0;
        }
    }

    program->v[program->size] = inst;
    program->size++;

    return 1;
}

void program_print(Program *program) {
    Instruction *v = program->v;

    for (size_t i = 0; i < program_size(program); i++) {
        inst_print(v[i], i);
    }
}

void inst_print(Instruction inst, size_t index) {
    printf("[%zu]: %i %i %i %i\n", index, inst.code,
        inst.dest, inst.arg1, inst.arg2);
}

Instruction *program_fetch(Program *program, size_t index) {
    return &program->v[index];
}
