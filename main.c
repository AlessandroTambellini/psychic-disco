#include <stdio.h>
#include "program.h"
#include "vm.h"

int main()
{
    // Create program
    Program program;
    program_init(&program);

    Instruction i1 = { MOVI, 0, 0 };
    Instruction i2 = { BEQI, 4, 0, 420 };
    Instruction i3 = { ADDI, 0, 0, 1 };
    Instruction i4 = { B, 1 };
    Instruction i5 = { MOVI, 1, 0 };

    program_add(&program, i1);
    program_add(&program, i2);
    program_add(&program, i3);
    program_add(&program, i4);
    program_add(&program, i5);

    // Create vm
    Vm vm;
    vm_init(&vm, &program);

    // Fetch-execute loop
    loop(&vm);

    memory_print(&vm);

    return 0;
}
