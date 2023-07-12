#include <stdio.h>

#ifdef __linux__
#include <sys/sysinfo.h>
#else
#include <windows.h>
#endif

#include "interpreter.h"
#include "program.h"

int intprt_init(Interpreter *intprt, Program program, unsigned long long *intprt_mem)
{
    intprt->program = program;
    intprt->counter = 0;

#ifdef __linux__

    struct sysinfo sys_info;
    int res = sysinfo(&sys_info);
    if (res != -1)
    {
        *intprt_mem = (unsigned long)sys_info.freeram;
        intprt->data = (int *)malloc(*intprt_mem);
    }
    else
    {
        printf("Failed to allocate interpreter memory.\n");
        return 0;
    }

#else

    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status))
    {
        printf("status.ullAvaiablePhys: %llu\n", status.ullAvailPhys);
        *intprt_mem = (unsigned long long)status.ullAvailPhys;
        intprt->data = (int *)malloc(*intprt_mem);
    }
    else
    {
        printf("Failed to allocate interpreter memory.\n");
        return 0;
    }

#endif

    return 1;
}

// Memory
void memory_print(Interpreter *intprt)
{
    int limit = 20;
    for (int i = 0; i < limit && i < DATA_SIZE; i++)
    {
        printf("[%i]: %i\n", i, intprt->data[i]);
    }
}

// Fetch-execute loop
void loop(Interpreter *intprt)
{
    Instruction *inst;

    while (intprt->counter < intprt->program.size)
    {
        // Fetch instruction
        inst = fetch(intprt);

        // Increment program counter
        intprt->counter++;

        // Execute instruction
        execute(intprt, inst);
    }
}

Instruction *fetch(Interpreter *intprt)
{
    size_t counter = intprt->counter;
    return program_fetch(&intprt->program, counter);
}

// Pointer to interpreter cause we need
// to increase the program counter
void execute(Interpreter *intprt, Instruction *inst)
{
    int dest = inst->dest;
    int arg1 = inst->arg1;
    int arg2 = inst->arg2;

    switch (inst->code)
    {
    case 0:
        add(intprt, dest, arg1, arg2);
        break;
    case 1:
        addi(intprt, dest, arg1, arg2);
        break;
    case 2:
        sub(intprt, dest, arg1, arg2);
        break;
    case 3:
        subi(intprt, dest, arg1, arg2);
        break;
    case 4:
        addi(intprt, dest, arg1, 0);
        break; // mov implementation
    case 5:
        movi(intprt, dest, arg1);
        break;
    case 6:
        beq(intprt, dest, 0, 0);
        break; // b implementation
    case 7:
        beq(intprt, dest, arg1, arg2);
        break;
    case 8:
        beqi(intprt, dest, arg1, arg2);
        break;
    case 9:
        bne(intprt, dest, arg1, arg2);
        break;
    case 10:
        bnei(intprt, dest, arg1, arg2);
        break;
    default:
        printf("default\n");
        break;
    }
}

// Instructions
void add(Interpreter *intprt, int dest, int arg1, int arg2)
{
    intprt->data[dest] = intprt->data[arg1] + intprt->data[arg2];
}

void addi(Interpreter *intprt, int dest, int arg1, int arg2)
{
    intprt->data[dest] = intprt->data[arg1] + arg2;
}

void sub(Interpreter *intprt, int dest, int arg1, int arg2)
{
    intprt->data[dest] = intprt->data[arg1] - intprt->data[arg2];
}

void subi(Interpreter *intprt, int dest, int arg1, int arg2)
{
    intprt->data[dest] = intprt->data[arg1] - arg2;
}

void movi(Interpreter *intprt, int dest, int arg1)
{
    intprt->data[dest] = arg1;
}

void beq(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (arg1 == arg2)
        intprt->counter = dest;
    else if (intprt->data[arg1] == intprt->data[arg2])
        intprt->counter = dest;
}

void beqi(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (intprt->data[arg1] == arg2)
        intprt->counter = dest;
}

void bne(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (arg1 != arg2 && intprt->data[arg1] != intprt->data[arg2])
        intprt->counter = dest;
}

void bnei(Interpreter *intprt, int dest, int arg1, int arg2)
{
    if (intprt->data[arg1] != arg2)
    {
        intprt->counter = dest;
    }
}
