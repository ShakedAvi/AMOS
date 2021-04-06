#include "stack.h"

/*
  The function initializes a stack.

  Input:
    The stack to initialize.
  Output:
    None.
*/
void init_stack(stack_t* stack)
{
    int* content;

    content = (int*)kmalloc(sizeof(int)*2);

    if (content == 0)
    {
        print("Not enough memory to initialize stack\n");
        return;
    }

    stack->content = content;
    stack->current_size = 2;
    stack->top = -1;
}

/*
  The function destroys a stack.

  Input:
    The stack to destroy.
  Output:
    None.
*/
void destroy(stack_t* stack)
{
    kfree(stack->content);

    stack->content = 0;
    stack->current_size = 0;
    stack->top = -1;
}

/*
  The function resizes a stack.

  Input:
    The stack to resize.
  Output:
    None.
*/
void resize(stack_t* stack)
{
    int* resized_content;
    resized_content = (int*)krealloc(stack->content, sizeof(int) * stack->current_size, sizeof(int) * stack->current_size * 2);

    stack->content = resized_content;
    stack->current_size = stack->current_size * 2;
}

/*
  The function checks if a stack is empty.

  Input:
    The stack to check.
  Output:
    If the stack is empty (1 if it is).
*/
int is_empty(stack_t* stack)
{
    if (stack->top < 0)
    {
      return 1;
    }
    else
    {
      return 0;
    }
}

/*
  The function checks if a stack is full.

  Input:
    The stack to check.
  Output:
    If the stack is full (1 if it is).
*/
int is_full(stack_t* stack)
{
    if (stack->top == stack->current_size - 1)
    {
      return 1;
    }
    else
    {
      return 0;
    }
}

/*
  The function pushes a new value to a stack.

  Input:
    The value to push and the stack to push into.
  Output:
    None.
*/
void push(stack_t* stack, int data)
{
    if (is_full(stack) == 1)
    {
      resize(stack);
    }

    stack->top = stack->top + 1;
    stack->content[stack->top] = data;
}

/*
  The function returns the top element of a stack.

  Input:
    The stack to return it's top element.
  Output:
    The top element of the given stack.
*/
int peek(stack_t* stack)
{
    return stack->content[stack->top];
}

/*
  The function returns the top element of a stack and removes it.

  Input:
    The stack to pop it's top element.
  Output:
    The top element of the given stack.
*/
int pop(stack_t* stack)
{
    if (is_empty(stack) == 0)
    {
        int data = stack->content[stack->top];
        stack->top = stack->top - 1;
        return data;
    }

    return -1;
}
