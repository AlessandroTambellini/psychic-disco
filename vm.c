#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "program.h"

// Interpreter
void vm_init(Vm *vm)
{
    Program *program = (Program *)malloc(sizeof(Program));
    program_init(program);
    vm->program = program;

    // Setup registers
    vm_setreg(vm);
}

void vm_deinit(Vm *vm)
{
    program_deinit(vm->program);
    free(vm->program);
}

void vm_setreg(Vm *vm)
{
    vm->data[R0] = 0;
    vm->data[R1] = 0;
    vm->data[R2] = 0;
    vm->data[R3] = 0;
    vm->data[PC] = 0;
    vm->data[SP] = SB;
    vm->data[BP] = SB;
}

// Memory
void memory_print(Vm *vm)
{
    int limit = 16;
    for (int i = 0; i < limit && i < DATA_SIZE; i++) {
        printf("[%i]:\t%i\n", i, vm->data[i]);
    }
}

// Fetch-execute loop
void loop(Vm *vm)
{
    Instruction *inst;
    while (vm->data[PC] < program_size(vm->program)) {
        // Fetch instruction
        inst = fetch(vm);

        // Increment program counter
        vm->data[PC]++;

        // Execute instruction
        InstResult res = execute(vm, inst);

        // Handle result
        if (res != OK) {
            printf("Error: %s at instruction %d\n", res_names[res], vm->data[PC]);
            break;
        }
    }
}

// Fetch-execute loop nonblocking
bool loopn(Vm *vm)
{
    Instruction *inst;
    size_t count = 0;
    while (1) {
        // Fetch instruction
        inst = fetch(vm);

        // Increment program counter
        vm->data[PC]++;
        count++;

        // Execute instruction
        InstResult res = execute(vm, inst);

        // Handle result
        if (res != OK) {
            printf("Error: %s at instruction %d\n", res_names[res], vm->data[PC]);
            break;
        }

        // Check if program finished
        if (vm->data[PC] >= program_size(vm->program)) {
            return true;
        }

        // Check if execution exceeded context size
        if (count >= CONTEXT_SIZE) {
            printf("An infinite loop was detected\n");
            return false;
        }
    }

    return true;
}

Instruction *fetch(Vm *vm)
{
    return program_fetch(vm->program, vm->data[PC]);
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
    case DIV:   res = ddiv(vm, dest, arg1, arg2); break;
    case DIVI:  res = divi(vm, dest, arg1, arg2); break;
    case MOV:   res = addi(vm, dest, arg1, 0); break;
    case MOVI:  res = movi(vm, dest, arg1); break;
    case PUSH:  res = push(vm, dest); break;
    case PUSHI: res = pushi(vm, dest); break;
    case POP:   res = pop(vm, dest); break;
    case B:     res = beq(vm, dest, 0, 0); break;
    case BEQ:   res = beq(vm, dest, arg1, arg2); break;
    case BEQI:  res = beqi(vm, dest, arg1, arg2); break;
    case BNE:   res = bne(vm, dest, arg1, arg2); break;
    case BNEI:  res = bnei(vm, dest, arg1, arg2); break;
    case BGE:   res = bge(vm, dest, arg1, arg2); break;
    case BGEI:  res = bgei(vm, dest, arg1, arg2); break;
    case BLEI:  res = bgei(vm, dest, arg2, arg1); break;
    case RET:   res = ret(vm, dest); break;
    case RETI:  res = reti(vm, dest); break;
    case HALT:  res = halt(vm); break;
    default:
        res = MALFORMED_INSTRUCTION;
        break;
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

InstResult ddiv(Vm *vm, int dest, int arg1, int arg2)
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
    if (CHECK_DATA_BOUNDS(dest)) {
        vm->data[dest] = arg1;
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult push(Vm *vm, int dest)
{
    if (CHECK_DATA_BOUNDS_2(vm->data[SP], dest)) {
        vm->data[vm->data[SP]] = vm->data[dest];
        vm->data[SP]++;
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult pushi(Vm *vm, int dest)
{
    if (CHECK_DATA_BOUNDS(vm->data[SP])) {
        vm->data[vm->data[SP]] = dest;
        vm->data[SP]++;
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult pop(Vm *vm, int dest)
{
    if (CHECK_DATA_BOUNDS(dest)
        && vm->data[SP] > 0) {
        vm->data[SP]--;
        vm->data[dest] = vm->data[vm->data[SP]];
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult beq(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_2(arg1, arg2)
        && dest < program_size(vm->program)) {
        if (arg1 == arg2)
            vm->data[PC] = dest;
        else if (vm->data[arg1] == vm->data[arg2])
            vm->data[PC] = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult beqi(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS(arg1)
        && dest < program_size(vm->program)) {
        if (vm->data[arg1] == arg2)
            vm->data[PC] = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult bne(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_2(arg1, arg2)
        && dest < program_size(vm->program)) {
        if (vm->data[arg1] != vm->data[arg2])
            vm->data[PC] = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult bnei(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS(arg1)
        && dest < program_size(vm->program)) {
        if (vm->data[arg1] != arg2)
            vm->data[PC] = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult bge(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS_2(arg1, arg2)
        && dest < program_size(vm->program)) {
        if (vm->data[arg1] >= vm->data[arg2])
            vm->data[PC] = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult bgei(Vm *vm, int dest, int arg1, int arg2)
{
    if (CHECK_DATA_BOUNDS(arg1)
        && dest < program_size(vm->program)) {
        if (vm->data[arg1] >= arg2)
            vm->data[PC] = dest;

        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult ret(Vm *vm, int dest)
{
    if (CHECK_DATA_BOUNDS(dest)) {
        vm->data[0] = vm->data[dest];
        return OK;
    }

    return DATA_OVERFLOW;
}

InstResult reti(Vm *vm, int dest)
{
    vm->data[0] = dest;
    return OK;
}

InstResult halt(Vm *vm)
{
    vm->data[PC] = program_size(vm->program);
    return OK;
}

#undef CHECK_DATA_BOUNDS
#undef CHECK_DATA_BOUNDS_2
#undef CHECK_DATA_BOUNDS_3
