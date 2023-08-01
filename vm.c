#include <stdio.h>
#include "program.h"
#include "vm.h"

#define STRINGIFY(res) #res

static const char *res_names[] = {
    STRINGIFY(OK),
    STRINGIFY(DATA_OVERFLOW),
    STRINGIFY(MALFORMED_INSTRUCTION),
    STRINGIFY(DIVISION_BY_ZER)
};

#undef STRINGIFY

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

        // Handle result
        if (res != OK) {
            printf("Error: %s at instruction %d.\n", res_names[res], vm->pc);
            return;
        }
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
        case MUL:   res = mul(vm, dest, arg1, arg2); break;
        case MULI:  res = muli(vm, dest, arg1, arg2); break;
        case DIV:   res = div(vm, dest, arg1, arg2); break;
        case DIVI:  res = divi(vm, dest, arg1, arg2); break;
        case MOV:   res = addi(vm, dest, arg1, 0); break;
        case MOVI:  res = movi(vm, dest, arg1); break;
        case B:     res = beq(vm, dest, 0, 0); break;
        case BEQ:   res = beq(vm, dest, arg1, arg2); break;
        case BEQI:  res = beqi(vm, dest, arg1, arg2); break;
        case BNE:   res = bne(vm, dest, arg1, arg2); break;
        case BNEI:  res = bnei(vm, dest, arg1, arg2); break;
        case BLEI:  res = blei(vm, dest, arg1, arg2); break;
        case BGEI:  res = bgei(vm, dest, arg1, arg2); break;
        case HALT:  res = halt(vm); break;
        default:    res = MALFORMED_INSTRUCTION; break;
    }

    return res;
}

#define CHECK_DATA_BOUNDS(arg) \
    arg >= 0 && arg < DATA_SIZE

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

    return DATA_OVERFLOW;
}

InstResult addi(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_2(dest, arg1)) {
        vm->data[dest] = vm->data[arg1] + arg2;
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult sub(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_3(dest, arg1, arg2)) {
        vm->data[dest] = vm->data[arg1] - vm->data[arg2];
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult subi(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_2(dest, arg1)) {
        vm->data[dest] = vm->data[arg1] - arg2;
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult mul(Vm *vm, int dest, int arg1, int arg2) 
{
    if (CHECK_DATA_BOUNDS_3(dest, arg1, arg2)) {
        vm->data[dest] = vm->data[arg1] * vm->data[arg2];
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult muli(Vm *vm, int dest, int arg1, int arg2) 
{
    if (CHECK_DATA_BOUNDS_2(dest, arg1)) {
        vm->data[dest] = vm->data[arg1] * arg2;
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult div(Vm *vm, int dest, int arg1, int arg2) 
{
    if (CHECK_DATA_BOUNDS_3(dest, arg1, arg2)) {
        int den = vm->data[arg2];
        if (den == 0)
            return DIVISION_BY_ZERO;

        vm->data[dest] = vm->data[arg1] / den;
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult divi(Vm *vm, int dest, int arg1, int arg2) 
{
    if (arg2 == 0)
        return DIVISION_BY_ZERO;

    if (CHECK_DATA_BOUNDS_2(dest, arg1)) {
        vm->data[dest] = vm->data[arg1] / arg2;
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult movi(Vm *vm, int dest, int arg1)
{
    if (CHECK_DATA_BOUNDS_2(dest, arg1)) {
        vm->data[dest] = arg1;
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult beq(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_2(arg1, arg2)
            && dest < program_size(&vm->program)) {
        if (arg1 == arg2)
            vm->pc = dest;
        else if (vm->data[arg1] == vm->data[arg2])
            vm-> pc = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult beqi(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS(arg1)
            && dest < program_size(&vm->program)) {
        if (vm->data[arg1] == arg2)
            vm->pc = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult bne(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_2(arg1, arg2)
            && dest < program_size(&vm->program)) {
        if (vm->data[arg1] != vm->data[arg2])
            vm-> pc = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult bnei(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS(arg1)
            && dest < program_size(&vm->program)) {
        if (vm->data[arg1] != arg2)
            vm->pc = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult blei(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS(arg1)
            && dest < program_size(&vm->program)) {
        if (vm->data[arg1] <= arg2)
            vm->pc = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult bgei(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS(arg1)
            && dest < program_size(&vm->program)) {
        if (vm->data[arg1] >= arg2)
            vm->pc = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult halt(Vm *vm)
{
    vm->pc = program_size(&vm->program);
    return OK;
}

#undef CHECK_DATA_BOUNDS_2
#undef CHECK_DATA_BOUNDS_3
