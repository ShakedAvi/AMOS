#ifndef STACK_H
#define STACK_H

#include "paging.h"

typedef struct
{
    int* content;
    int current_size;
    int top;
} Stack;

void init_stack(Stack* stack);

void destroy(Stack* stack);

void resize(Stack* stack);

int is_empty(Stack* stack);

int is_full(Stack* stack);

void push(Stack* stack, int data);

int peek(Stack* stack);

int pop(Stack* stack);

#endif
