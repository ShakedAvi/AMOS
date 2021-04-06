#ifndef STACK_H
#define STACK_H

#include "paging.h"

typedef struct stack
{
    int* content;
    int current_size;
    int top;
}stack_t;

/*
  The function initializes a stack.

  Input:
    The stack to initialize.
  Output:
    None.
*/
void init_stack(stack_t* stack);

/*
  The function destroys a stack.

  Input:
    The stack to destroy.
  Output:
    None.
*/
void destroy(stack_t* stack);

/*
  The function resizes a stack.

  Input:
    The stack to resize.
  Output:
    None.
*/
void resize(stack_t* stack);

/*
  The function checks if a stack is empty.

  Input:
    The stack to check.
  Output:
    If the stack is empty (1 if it is).
*/
int is_empty(stack_t* stack);

/*
  The function checks if a stack is full.

  Input:
    The stack to check.
  Output:
    If the stack is full (1 if it is).
*/
int is_full(stack_t* stack);

/*
  The function pushes a new value to a stack.

  Input:
    The value to push and the stack to push into.
  Output:
    None.
*/
void push(stack_t* stack, int data);

/*
  The function returns the top element of a stack.

  Input:
    The stack to return it's top element.
  Output:
    The top element of the given stack.
*/
int peek(stack_t* stack);

/*
  The function returns the top element of a stack and removes it.

  Input:
    The stack to pop it's top element.
  Output:
    The top element of the given stack.
*/
int pop(stack_t* stack);

#endif
