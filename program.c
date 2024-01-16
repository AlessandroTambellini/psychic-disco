#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "program.h"

bool program_init(Program *program)
{
    program->v = (Instruction *)malloc(sizeof(Instruction));
    if (program->v == NULL)
        return false;

    program->capacity = 1;
    program->size = 0;

    return true;
}

bool program_deinit(Program *program)
{
    if (program == NULL)
        return false;

    if (program->v == NULL) {
        return false;
    } else {
        free(program->v);
    }

    return true;
}

Instruction *program_data(Program *program)
{
    return program->v;
}

size_t program_capacity(Program *program)
{
    return program->capacity;
}

size_t program_size(Program *program)
{
    return program->size;
}

bool program_resize(Program *program, size_t capacity_new)
{
    Instruction *v_new = realloc(program->v, capacity_new * sizeof(Instruction));
    if (v_new == NULL) {
        printf("Memory reallocation failed.\n");
        return false;
    }

    program->v = v_new;
    program->capacity = capacity_new;

    return true;
}

bool program_inc_capacity(Program *program)
{
    size_t capacity_new = 2 * program->capacity;
    return program_resize(program, capacity_new);
}

bool program_clear(Program *program)
{
    program->size = 0;
    return true;
}

bool program_merge(Program *program1, Program *program2)
{
    bool rv = program_resize(program1, program1->capacity + program2->capacity);
    if (!rv) {
        printf("Failed resize during program merge.\n");
        return false;
    }

    Instruction *dst = program1->v + program1->size;
    Instruction *src = program2->v;
    size_t src_size = program2->size * sizeof(Instruction);

    memcpy(dst, src, src_size);
    program1->size += program2->size;

    return true;
}

// Pointer to program cause we may need to
// re-allocate the Instruction vector;
bool program_add(Program *program, Instruction inst)
{
    if (program->size == program->capacity) {
        if (!program_inc_capacity(program)) {
            printf("Failed to increase capacity.\n");
            return false;
        }
    }

    program->v[program->size] = inst;
    program->size++;

    return true;
}

Instruction *program_fetch(Program *program, size_t index)
{
    return &program->v[index];
}

void program_print(Program *program)
{
    Instruction *v = program->v;

    for (size_t i = 0; i < program->size; i++) {
        inst_print(v[i], i);
    }
}

void inst_print(Instruction inst, size_t index)
{
    printf("[%zu]:\t%i %i %i %i\n", index, inst.code,
        inst.dest, inst.arg1, inst.arg2);
}
