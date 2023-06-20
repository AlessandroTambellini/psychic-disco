#include <stdio.h>
#include "program.h"

int main() {
    Program *program;
    program_init(program);

    Instruction i1 = { 1, 0, 0, 0 };
    Instruction i2 = { 2, 0, 0, 0 };
    Instruction i3 = { 3, 0, 0, 0 };
    Instruction i4 = { 4, 0, 0, 0 };
    Instruction i5 = { 5, 0, 0, 0 };

    program_add(program, i1);
    program_add(program, i2);
    program_add(program, i3);
    program_add(program, i4);
    program_add(program, i5);

    program_print(program);

    return 0;
}
