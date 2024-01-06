#include "program.h"
#include "vm.h"

int main()
{
    // Create program (factorial of 5)
    Program program1, program2;
    program_init(&program1);
    program_init(&program2);

    Instruction i1 = { MOVI,    0, 5 };
    Instruction i2 = { MOV,     1, 0 };
    Instruction i3 = { ADDI,    1, 1, -1};

    Instruction i4 = { BEQI,    6, 1, 1};
    Instruction i5 = { MUL,     0, 0, 1};
    Instruction i6 = { B,       2 };
    Instruction i7 = { HALT };

    program_add(&program1, i1);
    program_add(&program1, i2);
    program_add(&program1, i3);

    program_add(&program2, i4);
    program_add(&program2, i5);
    program_add(&program2, i6);
    program_add(&program2, i7);

    program_merge(&program1, &program2);
    program_print(&program1);

    // Create vm
    Vm vm;
    vm_init(&vm, &program1);

    // Fetch-execute loop
    loop(&vm);
    memory_print(&vm);

    // Unload program
    program_deinit(&program1);

    return 0;
}
