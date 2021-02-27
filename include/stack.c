#include "stack.h"

void init_stack(Stack* stack)
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

void destroy(Stack* stack)
{
    kfree(stack->content);

    stack->content = 0;
    stack->current_size = 0;
    stack->top = -1;
}

void resize(Stack* stack)
{
    int* resized_content;
    resized_content = (int*)krealloc(stack->content, sizeof(int) * stack->current_size, sizeof(int) * stack->current_size * 2);

    stack->content = resized_content;
    stack->current_size = stack->current_size * 2;
}

int is_empty(Stack* stack)
{
    if (stack->top < 0)
        return 1;
    else
        return 0;
}

int is_full(Stack* stack)
{
    if (stack->top == stack->current_size - 1)
        return 1;
    else
        return 0;
}

void push(Stack* stack, int data)
{
    if (is_full(stack) == 1)
        resize(stack);

    stack->top = stack->top + 1;
    stack->content[stack->top] = data;
}

int peek(Stack* stack)
{
    return stack->content[stack->top];
}

int pop(Stack* stack)
{
    if (is_empty(stack) == 0)
    {
        int data = stack->content[stack->top];
        stack->top = stack->top - 1;
        return data;
    }

    return -1;
}
