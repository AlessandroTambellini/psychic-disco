#include <stdio.h>

#include "program.h"
#include "interpreter.h"

int main()
{
    // Creating program
    Program program;
    program_init(&program);

    Instruction i1 = {5, 0, 0, 0};
    Instruction i2 = {8, 200, 0, 420};
    Instruction i3 = {1, 0, 0, 1};
    Instruction i4 = {10, 1, 0, 420};

    program_add(&program, i1);
    program_add(&program, i2);
    program_add(&program, i3);
    program_add(&program, i4);

    // Creating interpreter
    Interpreter intprt;
    intprt_init(&intprt, program);

    // Fetch-execute loop
    loop(&intprt);

    memory_print(&intprt);

    return 0;
}
