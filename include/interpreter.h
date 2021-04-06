#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "types.h"
#include "paging.h"
#include "keyboard.h"
#include "string.h"
#include "stack.h"

/*
  The function sets up the interpreter and loads a code into it.

  Input:
    The code to run.
  Output:
    None.
*/
void run_interpreter(char* code_to_run);

#endif
