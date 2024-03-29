#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "program.h"

bool program_init(Program *program)
{
    program->items = (Instruction *)malloc(sizeof(Instruction));
    if (program->items == NULL)
        return false;

    program->capacity = 1;
    program->size = 0;

    return true;
}

bool program_deinit(Program *program)
{
    if (program == NULL)
        return false;

    if (program->items == NULL) {
        return false;
    } else {
        free(program->items);
    }

    return true;
}

Instruction *program_data(Program *program)
{
    return program->items;
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
    Instruction *v_new = realloc(program->items, capacity_new * sizeof(Instruction));
    if (v_new == NULL) {
        printf("Failed to reallocate program vector\n");
        return false;
    }

    program->items = v_new;
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

bool program_copy(Program *program, Program *src)
{
    bool rv = program_resize(program, src->capacity);
    if (!rv) {
        return false;
    }

    memcpy(program->items, src->items, src->size * sizeof(Instruction));

    program->size = src->size;
    program->capacity = src->capacity;

    return true;
}

bool program_merge(Program *program, Instruction *src, size_t size)
{
    bool rv = program_resize(program, program->capacity + size);
    if (!rv) {
        return false;
    }

    Instruction *dst = program->items + program->size;

    memcpy(dst, src, size * sizeof(Instruction));
    program->size += size;

    return true;
}

bool program_split(Program *program, Instruction *dst, size_t size)
{
    memcpy(dst, program->items, size * sizeof(Instruction));
    size_t size_new = program->size - size;
    program->size = size_new;
    if (size_new) {
        memmove(program->items, program->items + size, size_new * sizeof(Instruction));
    }
    return true;
}

bool program_delete(Program *program, size_t start, size_t size)
{
    if (start + size >= program->size) {
        printf("Interval is outside of the program bounds\n");
        return false;
    }

    size_t remain = program->size - start - size;

    Instruction *dst = program->items + start;
    Instruction *src = program->items + start + size;
    memmove(dst, src, remain * sizeof(Instruction));

    program->size -= size;

    return true;
}

bool program_get(Program *program, Instruction *dst, size_t start, size_t size)
{
    if (start + size >= program->size) {
        printf("Interval is outside of the program bounds\n");
        return false;
    }

    memcpy(dst, program->items + start, size * sizeof(Instruction));
    return true;
}

// Pointer to program cause we may need to
// re-allocate the Instruction vector;
bool program_add(Program *program, Instruction inst)
{
    if (program->size == program->capacity) {
        if (!program_inc_capacity(program)) {
            return false;
        }
    }

    program->items[program->size] = inst;
    program->size++;

    return true;
}

Instruction *program_fetch(Program *program, size_t index)
{
    return &program->items[index];
}

void program_print(Program *program)
{
    Instruction *items = program->items;

    for (size_t i = 0; i < program->size; i++) {
        inst_print(items[i], i);
    }
}

void inst_print(Instruction inst, size_t index)
{
    printf("[0x%.4zx]: %i %i %i %i\n", index, inst.code,
        inst.dest, inst.arg1, inst.arg2);
}
