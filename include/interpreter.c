#include "interpreter.h"

Variable programVars[16];
uint32 numVars;

Stack programStack;

char* programLines[20];
int currentLine;
int maxLines;

char get(void)
{
  char c = readChar();
  return c;
} // get one byte

int getInt(char* toInt)
{
  return ascii_to_int(toInt);
}

/*uint32 find_variable(char name)
{
  for(int i = 0; i < numVars; i++)
  {
    if (programVars[i]->name == name)
    {
      return i;
    }
  }
  return -1;
}*/

/*uint32 statement()
{
  char currChar = ' ';
  int currVarIndex = 0;
  do
  {
    currChar = get();
    if(currChar >= 'A' && currChar <= 'Z') // Variable
    {
      currVarIndex = find_variable(c);
      if(currVarIndex == -1)
      {
        programVars[numVars].name = currChar;
      }

      do
      {
        currChar = get();
      }
      while(currChar = ' ')

      if (currChar == '=')
      {
        do
        {
          currChar = get();
        }
        while(currChar = ' ')

        if (currChar == 'f' || currChar == 't') // Bool
        {
          programVars[numVars].type = BOOL_TYPE;
          programVars[numVars].value = currChar;
          currChar = get();
          if (currChar != ';')
          {
            print("Syntax Error After Initializing Boolean!\n");
            return;
          }
        }
        else if(currChar == '\'') // Char
        {
          programVars[numVars].type = CHAR_TYPE;
          currChar = get();
          programVars[numVars].value = currChar;
          currChar = get();
          if (currChar != '\'')
          {
            print("Syntax Error After Initializing Char!\n");
            return;
          }
          currChar = get();
          if (currChar != ';')
          {
            print("Syntax Error After Initializing Char!\n");
            return;
          }
        }
        else // Int
        {
          programVars[numVars].type = CHAR_TYPE;
          programVars[numVars].value = getInt();
        }
      }
    }
    else // Expression
    {

    }
  }
  while(currChar != ';')
}*/

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

  if (strEql(currCommand, "push"))
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
      push(&programStack, getInt(currNum));
    }
  }
  else if(strEql(currCommand, "pop"))
  {
    if (currStr[i] == '\0')
    {
      pop(&programStack);
    }
  }
  else if(strEql(currCommand, "add"))
  {
    if (currStr[i] == '\0')
    {
      first = pop(&programStack);
      second = pop(&programStack);

      push(&programStack, first + second);
    }
  }
  else if(strEql(currCommand, "dec"))
  {
    if (currStr[i] == '\0')
    {
      first = pop(&programStack);
      second = pop(&programStack);

      push(&programStack, first - second);
    }
  }
  else if(strEql(currCommand, "mul"))
  {
    if (currStr[i] == '\0')
    {
      first = pop(&programStack);
      second = pop(&programStack);

      push(&programStack, first * second);
    }
  }
  else if(strEql(currCommand, "div"))
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
      currentLine = getInt(currNum);
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
    currentLine = getInt(currNum);
  }
  else if(strEql(currCommand, "print"))
  {
    if (currStr[i] == '\0')
    {
      itoa(peek(&programStack), currPrint, 10);
      print(currPrint);
      print("\n");
    }
  }
  else if(strEql(currCommand, "dup"))
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
    maxLines++;
  }
  currentLine++;
}

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

    //print("\n");
    //print(currStr);
    //print("\n");

    /*print("\n");
    print(programLines[maxLines]);
    print("\n");
    print("\n");
    print(currStr);
    print("\n");*/
    programLines[maxLines] = (char*)kmalloc(20 * sizeof(char));
    strCpy(programLines[maxLines], currStr);

    maxLines++;

    //print("\n");
    //print(currStr);
    //print("\n");

    for(int k = 0; k < 20; k++)
    {
      currStr[k] = 0;
      //programLines[maxLines][k] = 0;
    }
  }

  /*print("\n");
  print(programLines[0]);
  print("\n");*/

  while(endFlag)
  {
    /*itoa(currentLine, printMe, 10);
    print("Current line = ");
    print(printMe);
    print("\n");

    itoa(maxLines, printMe, 10);
    print("Max lines = ");
    print(printMe);
    print("\n");*/

    print("\n");

    if (currentLine == maxLines)
    {
      print(">");
      readStr(currStr);
      print("\n");

      if(parse_command(currStr) == -1)
      {
        endFlag = 0;
      }
    }
    else
    {
      //print("Jumped to command: ");
      /*print("\n");
      print(programLines[currentLine]);
      print("\n");*/
      if(parse_command(programLines[currentLine]) == -1)
      {
        endFlag = 0;
      }
    }
    //print("\n");
  }
}
