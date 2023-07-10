#include <stdio.h>
#include <stdlib.h>

#include "program.h"
#include "interpreter.h"

int main()
{
    // Creating program
    Program program;
    program_init(&program);

    Instruction i1 = {5, 0, 0, 0};
    Instruction i2 = {8, 1000000000, 0, 420};
    Instruction i3 = {1, 0, 0, 1};
    Instruction i4 = {10, 1, 0, 420};

    // TODO: general memory limit
    program_add(&program, i1);
    program_add(&program, i2);
    program_add(&program, i3);
    program_add(&program, i4);

    // Creating interpreter
    Interpreter intprt;
    unsigned long long intprt_mem;
    intprt_init(&intprt, program, &intprt_mem);
    printf("intprt_mem: %llu\n", intprt_mem);

    // check all the instructions are valid after the memory for the interpreter is
    // allocated and before executing the program
    for (int i = 0; i < program.size; i++)
    {
        Instruction *inst = program_fetch(&intprt.program, i);
        if (inst->dest >= intprt_mem || inst->arg1 >= intprt_mem || inst->arg2 >= intprt_mem)
        {
            printf("Error in instruction {%d, %d, %d, %d}. At least one among dest, arg1, arg2 "
                   "exceeds the memory address limit.",
                   inst->code, inst->dest, inst->arg1, inst->arg2);
            exit(1);
            // TODO: ask the user to insert the istructions again instead of exiting the program
        }
    }

    // Fetch-execute loop
    loop(&intprt);

    memory_print(&intprt);

    return 0;
}
