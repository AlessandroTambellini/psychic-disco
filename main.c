#include <stdio.h>
#include "vector.h"

int main() {
    Vector *v;
    vector_init(v);
    printf("%zu\n", v->i_size);

    Instruction i1 = { 1, 2, 3, 4 };
    Instruction *pi1 = &i1;
    instructions_add(v, pi1);
    return 0;
}
