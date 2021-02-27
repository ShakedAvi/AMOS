#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "types.h"
#include "paging.h"
#include "keyboard.h"
#include "string.h"
#include "stack.h"

enum Types { BOOL_TYPE, INT_TYPE, FLOAT_TYPE, CHAR_TYPE, STRING_TYPE };

typedef struct Variable
{
  char name;
  uint32 type;
  uint32 value;
}Variable;

Variable programVars[16];
uint32 num_vars;

uint32 find_variable(char* name);

uint32 factor();
uint32 term();
uint32 statement();
uint32 expression();

void run_interpreter(char* code_to_run);

#endif
