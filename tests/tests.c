#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>

#include "../program.h"
#include "../utils.h"
#include "../vm.h"

void test_program_factorial()
{
    const int n = 10;
    int expected = 1;
    int m = n;
    while (m) expected *= m--;

    Vm vm;
    vm_init(&vm);

    Program program;
    program_init(&program);

    Instruction i0 = { MOVI,    R1, n };
    Instruction i1 = { MOV,     R0, R1 };
    Instruction i2 = { SUBI,    R1, R1, 1 };
    Instruction i3 = { BEQI,    6, R1, 1 };
    Instruction i4 = { MUL,     R0, R0, R1 };
    Instruction i5 = { B,       2 };
    Instruction i6 = { HALT };

    program_add(&program, i0);
    program_add(&program, i1);
    program_add(&program, i2);
    program_add(&program, i3);
    program_add(&program, i4);
    program_add(&program, i5);
    program_add(&program, i6);

    program_merge(vm.program, program.items, program.size);

    // Assert equality of the two programs
    for (size_t i = 0; i < program.size; i++) {
        Instruction *pi1 = program_fetch(vm.program, i);
        Instruction *pi2 = program_fetch(&program, i);
        assert(inst_eq(pi1, pi2));
    }

    // Assert termination
    assert(loopn(&vm));

    // Assert first element is factorial
    assert(vm.data[0] == expected);
}

int main()
{
    test_program_factorial();
}
