#include <stdio.h>
#include "stack.h"

bool stack_init(Stack *s)
{
    s->head = s->v;
    return true;
}

bool stack_deinit(Stack *s)
{
    return true;
}

size_t stack_size(Stack *s)
{
    return STACK_SIZE;
}

bool stack_push(Stack *s, int value)
{
    if (s->head == (s->v + STACK_SIZE))
        return false;

    *(s->head++) = value;

    return true;
}

int stack_pop(Stack *s)
{
    if (s->head != s->v)
        return *(--s->head);

    return 0;
}

void stack_print(Stack *s)
{
    int i = 0;
    for (int *c = s->v; c != s->head; c++) {
        printf("[%d](%p):\t %d\n", i, c, *c);
        i++;
    }
}
