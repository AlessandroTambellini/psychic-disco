#ifndef STACK_H
#define STACK_H

#include "stdbool.h"
#include "stddef.h"

#define STACK_SIZE 1000

typedef struct {
    int v[STACK_SIZE];
    int *head;
} Stack;

bool stack_init(Stack *);
bool stack_deinit(Stack *);
size_t stack_size(Stack *);
bool stack_push(Stack *, int);
int stack_pop(Stack *);
void stack_print(Stack *);

#endif
