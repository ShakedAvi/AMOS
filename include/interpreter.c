#include "interpreter.h"

stack_t programStack;

char* programLines[20];
int currentLine;
int maxLines;

/*
  The function interpretes a command on the machine.

  Input:
    The command to interpret.
  Output:
    Return code (-1 if the interpret went wrong).
*/
int32 parse_command(char* currStr)
{
  char currCommand[10] = { 0 };
  char currNum[10] = { 0 };
  char currPrint[10] = { 0 };
  int i = 0, j = 0, first = 0, second = 0;

  while(i < 10 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != ';')
  {
    currCommand[i] = currStr[i];
    i++;
  }

  currCommand[i] = 0;

  if (strEql(currCommand, "push")) // Push a new value to the stack
  {
    if (currStr[i] = ' ')
    {
      i++;
      while(j < 10 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != ';')
      {
        currNum[j] = currStr[i];
        i++;
        j++;
      }
      push(&programStack, ascii_to_int(currNum));
    }
  }
  else if(strEql(currCommand, "pop")) // Pop a value from the stack
  {
    if (currStr[i] == '\0')
    {
      pop(&programStack);
    }
  }
  else if(strEql(currCommand, "add")) // Add the two top values of the stack
  {
    if (currStr[i] == '\0')
    {
      first = pop(&programStack);
      second = pop(&programStack);

      push(&programStack, first + second);
    }
  }
  else if(strEql(currCommand, "sub")) // Subtract the two top values of the stack
  {
    if (currStr[i] == '\0')
    {
      first = pop(&programStack);
      second = pop(&programStack);

      push(&programStack, first - second);
    }
  }
  else if(strEql(currCommand, "mul")) // Multiply the two top values of the stack
  {
    if (currStr[i] == '\0')
    {
      first = pop(&programStack);
      second = pop(&programStack);

      push(&programStack, first * second);
    }
  }
  else if(strEql(currCommand, "div")) // Divide the two top values of the stack
  {
    if (currStr[i] == '\0')
    {
      first = pop(&programStack);
      second = pop(&programStack);

      push(&programStack, first / second);
    }
  }
  else if(strEql(currCommand, "ifeq")) // if the top of the stack is 0, jump to <address> (a line number)
  {
    if (peek(&programStack) == 0)
    {
      i++;
      while(j < 10 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != ';')
      {
        currNum[j] = currStr[i];
        i++;
        j++;
      }
      currentLine = ascii_to_int(currNum);
    }
  }
  else if(strEql(currCommand, "jump")) // jump to <address> (a line number)
  {
    i++;
    while(j < 10 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != ';')
    {
      currNum[j] = currStr[i];
      i++;
      j++;
    }
    print("\nJumping to: ");
    print(currNum);
    print("\n");

    currentLine = ascii_to_int(currNum);
  }
  else if(strEql(currCommand, "print")) // Print the top value of the stack
  {
    if (currStr[i] == '\0')
    {
      itoa(peek(&programStack), currPrint, 10);
      print(currPrint);
      print("\n");
    }
  }
  else if(strEql(currCommand, "dup")) // Duplicate the top value of the stack
  {
    if (currStr[i] == '\0')
    {
      push(&programStack, peek(&programStack));
    }
  }
  else
  {
    print("No Valid Command!\n");
    return -1;
  }

  if(currentLine == maxLines)
  {
    programLines[currentLine] = (char*)kmalloc(strLen(currCommand) * sizeof(char));
    strCpy(programLines[currentLine], currStr);
    programLines[currentLine][strLen(currStr)] = 0;
    maxLines++;
  }
  currentLine++;
}

/*
  The function sets up the interpreter and loads a code into it.

  Input:
    The code to run.
  Output:
    None.
*/
void run_interpreter(char* code_to_run)
{
  init_stack(&programStack);
  int endFlag = 1, i = 0, j = 0;
  currentLine = 0;
  maxLines = 0;

  char printMe[10] = { 0 };
  char currStr[20] = { 0 };

  while (code_to_run[i])
  {
    j = 0;
    while(code_to_run[i] != 0 && code_to_run[i] != ';' && j < 20)
    {
      currStr[j] = code_to_run[i];
      i++;
      j++;
    }

    currStr[j] = 0;
    i++;

    programLines[maxLines] = (char*)kmalloc(20 * sizeof(char));
    strCpy(programLines[maxLines], currStr);

    maxLines++;

    for(int k = 0; k < 20; k++)
    {
      currStr[k] = 0;
    }
  }

  while(endFlag)
  {
    print("\n");

    if (currentLine == maxLines)
    {
      print(">");
      readStr(currStr);
      print("\n");

      if(strEql(currStr, "exit"))
      {
        print("\nQuit!\n");
        destroy(&programStack);
        return;
      }

      if(parse_command(currStr) == -1)
      {
        endFlag = 0;
      }
    }
    else
    {
      if(parse_command(programLines[currentLine]) == -1)
      {
        endFlag = 0;
      }
    }
  }
}
