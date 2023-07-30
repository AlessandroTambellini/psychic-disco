#include <stdio.h>

#include "program.h"
#include "interpreter.h"

int main() {
    // Creating program
    Program program;
    program_init(&program);

    Instruction i1 = { 5, 0, 0, 0 };
    Instruction i2 = { 8, 27, 0, 420 };
    Instruction i3 = { 1, 0, 0, 1 };
    Instruction i4 = { 6, 1, 0, 0 };

    program_add(&program, i1);
    program_add(&program, i2);
    program_add(&program, i3);
    program_add(&program, i4);

    // Creating interpreter
    Interpreter intprt;
    intprt.program = program;
    intprt.counter = 0;

    // Fetch-execute loop
    loop(&intprt);

    memory_print(&intprt);

    return 0;
}
