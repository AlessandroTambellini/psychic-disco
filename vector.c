#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

int vector_init(Vector *v) {
    v->instructions = (Instruction *) malloc(sizeof(Instruction));
    if (v->instructions == NULL)
        return 0;

    v->i_capacity = 1;
    v->i_size = 0;

    v->d_capacity = 1;
    v->d_size = 0;

    v->data = (Data *) malloc(sizeof(Data));
    if (v->data == NULL)
        return 0;
    else
        return 1;
}

int vector_deinit(Vector *v) {
    if (v == NULL)
        return 0;

    if (v->instructions == NULL) {
        free(v);
        return 0;
    }
    else {
        free(v->instructions);
    }

    if (v->data == NULL) {
        free(v);
        return 0;
    }
    else {
        free(v->data);
    }

    free(v);

    return 1;
}

size_t instructions_size(Vector *v) {
    return v->i_size;
}

int instructions_resize(Vector *v) {
    Instruction *instructions_old = v->instructions;

    size_t capacity_new = 2 * v->i_capacity;
    printf("%lu\n", capacity_new);
    v->instructions = (Instruction *) malloc(capacity_new * sizeof(Instruction));
    if (v->instructions == NULL)
        return 0;

    for (size_t i = 0; i < v->i_size; i++) {
        v->instructions[i] = instructions_old[i];
    }

    v->i_capacity = capacity_new;

    return 1;
}

int instructions_add(Vector *v, Instruction *instr) {
    if (v->i_size == v->i_capacity) {
        if (instructions_resize(v)) 
            return 0;
    }

    v->instructions[v->i_size] = *instr;
    v->i_size++;

    return 1;
}
