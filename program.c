#include <stdlib.h>
#include <stdio.h>
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

size_t program_size(Program *program)
{
    return program->size;
}

bool program_inc_capacity(Program *program)
{
    size_t capacity_new = 2 * program->capacity;

    Instruction *v_new = realloc(program->v, capacity_new * sizeof(Instruction));
    if (v_new == NULL) {
        printf("Memory reallocation failed.\n");
        return false;
    }

    program->v = v_new;
    program->capacity = capacity_new;

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

void program_print(Program *program)
{
    Instruction *v = program->v;

    for (size_t i = 0; i < program_size(program); i++)
    {
        inst_print(v[i], i);
    }
}

void inst_print(Instruction inst, size_t index)
{
    printf("[%zu]:\t%i %i %i %i\n", index, inst.code,
        inst.dest, inst.arg1, inst.arg2);
}

Instruction *program_fetch(Program *program, size_t index)
{
    return &program->v[index];
}
