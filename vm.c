#include <stdio.h>
#include "program.h"
#include "vm.h"

static char *res_names[] = {
    "OK",
    "OVERFLOW",
    "MALFORMED_INSTRUCTION"
};

// Interpreter
void vm_init(Vm *vm, Program *program) {
    vm->program = *program;
    vm->pc = 0;
}

// Memory
void memory_print(Vm *vm)
{
    int limit = 16;
    for (int i = 0; i < limit && i < DATA_SIZE; i++) {
        printf("[%i]:\t %i\n", i, vm->data[i]);
    }
}

// Fetch-execute loop
void loop(Vm *vm)
{
    Instruction *inst;

    while (vm->pc < vm->program.size) {
        // Fetch instruction
        inst = fetch(vm);

        // Increment program counter
        vm->pc++;

        // Execute instruction
        InstResult res = execute(vm, inst);
        // printf("%s\n", res_names[res]);
    }
}

Instruction *fetch(Vm *vm)
{
    size_t pc = vm->pc;
    return program_fetch(&vm->program, pc);
}

// Pointer to interpreter cause we need
// to increase the program counter
InstResult execute(Vm *vm, Instruction *inst)
{
    int dest = inst->dest;
    int arg1 = inst->arg1;
    int arg2 = inst->arg2;

    InstResult res;
    switch (inst->code) {
        case ADD:   res = add(vm, dest, arg1, arg2); break;
        case ADDI:  res = addi(vm, dest, arg1, arg2); break;
        case SUB:   res = sub(vm, dest, arg1, arg2); break;
        case SUBI:  res = subi(vm, dest, arg1, arg2); break;
        case MOV:   res = addi(vm, dest, arg1, 0); break;
        case MOVI:  res = movi(vm, dest, arg1); break;
        case B:     res = beq(vm, dest, 0, 0); break;
        case BEQ:   res = beq(vm, dest, arg1, arg2); break;
        case BEQI:  res = beqi(vm, dest, arg1, arg2); break;
        case BNE:   res = bne(vm, dest, arg1, arg2); break;
        case BNEI:  res = bnei(vm, dest, arg1, arg2); break;
        default:    res = MALFORMED_INSTRUCTION; break;
    }

    return res;
}

#define CHECK_DATA_BOUNDS_2(arg1, arg2) \
    arg1 >= 0 && arg1 < DATA_SIZE \
    && arg2 >= 0 && arg2 < DATA_SIZE

#define CHECK_DATA_BOUNDS_3(arg1, arg2, arg3) \
    arg1 >= 0 && arg1 < DATA_SIZE \
    && arg2 >= 0 && arg2 < DATA_SIZE \
    && arg3 >= 0 && arg3 < DATA_SIZE

// Instructions
InstResult add(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_3(dest, arg1, arg2)) {
        vm->data[dest] = vm->data[arg1] + vm->data[arg2];
        return OK;
    }

    return OVERFLOW;
}

InstResult addi(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_2(dest, arg1)) {
        vm->data[dest] = vm->data[arg1] + arg2;
        return OK;
    }

    return OVERFLOW;
}

InstResult sub(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_3(dest, arg1, arg2)) {
        vm->data[dest] = vm->data[arg1] - vm->data[arg2];
        return OK;
    }

    return OVERFLOW;
}

InstResult subi(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_2(dest, arg1)) {
        vm->data[dest] = vm->data[arg1] - arg2;
        return OK;
    }

    return OVERFLOW;
}

InstResult movi(Vm *vm, int dest, int arg1)
{
    if (CHECK_DATA_BOUNDS_2(dest, arg1)) {
        vm->data[dest] = arg1;
        return OK;
    }

    return OVERFLOW;
}

InstResult beq(Vm *vm, int dest, int arg1, int arg2)
{
    if (dest < program_size(&vm->program)
            && CHECK_DATA_BOUNDS_2(arg1, arg2)) {
        if (arg1 == arg2)
            vm->pc = dest;
        else if (vm->data[arg1] == vm->data[arg2])
            vm-> pc = dest;

        return OK;
    }

    return OVERFLOW;
}

InstResult beqi(Vm *vm, int dest, int arg1, int arg2)
{
    if (arg1 < DATA_SIZE && dest < program_size(&vm->program)) {
        if (vm->data[arg1] == arg2)
            vm->pc = dest;

        return OK;
    }

    return OVERFLOW;
}

InstResult bne(Vm *vm, int dest, int arg1, int arg2)
{
    if (dest < program_size(&vm->program)
            && CHECK_DATA_BOUNDS_2(arg1, arg2)) {
        if (vm->data[arg1] != vm->data[arg2])
            vm-> pc = dest;

        return OK;
    }

    return OVERFLOW;
}

InstResult bnei(Vm *vm, int dest, int arg1, int arg2)
{
    if (arg1 < DATA_SIZE && dest < program_size(&vm->program)) {
        if (vm->data[arg1] != arg2)
            vm->pc = dest;

        return OK;
    }

    return OVERFLOW;
}

#undef CHECK_DATA_BOUNDS_2
#undef CHECK_DATA_BOUNDS_3
