#include "interpreter.h"
#include "program.h"

void execute(Interpreter intprt)
{
    Instruction *inst = fetch(intprt);
    switch (inst->code) {
        case 0: printf("0\n"); break;
        case 1: printf("1\n"); break;
        case 2: printf("2\n"); break;
        case 3: printf("3\n"); break;
        case 4: printf("4\n"); break;
        case 5: printf("5\n"); break;
    default: printf("default\n"); break;
    }
}

Instruction *fetch(Interpreter intprt) {
    return program_fetch(&intprt.program, intprt.counter);
}
