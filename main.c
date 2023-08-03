#include <stdio.h>
#include "program.h"
#include "vm.h"

int main()
{
    // Create program (factorial of 5)
    Program program;
    program_init(&program);

    Instruction i1 = { MOVI,    0, 5 };
    Instruction i2 = { MOV,     1, 0 };
    Instruction i3 = { ADDI,    1, 1, -1};
    Instruction i4 = { BEQI,    6, 1, 1};
    Instruction i5 = { MUL,     0, 0, 1};
    Instruction i6 = { B,       2 };
    Instruction i7 = { HALT };

    // the program counter counts from 0

    /* 
    r0: 5
    r1: 5
    r1: 4
    r0: 5 * 4 = 20

    r1: 3
    r0: 20 * 3 = 60

    r1: 2
    r0: 60 * 2 = 120

    pc = program->size
    exit the program
    */

    program_add(&program, i1);
    program_add(&program, i2);
    program_add(&program, i3);
    program_add(&program, i4);
    program_add(&program, i5);
    program_add(&program, i6);
    program_add(&program, i7);

    // Create vm
    Vm vm;
    vm_init(&vm, &program);

    // Fetch-execute loop
    loop(&vm);

    memory_print(&vm);

    return 0;
}
