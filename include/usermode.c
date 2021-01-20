#include "usermode.h"

extern void jump_usermode();

void jump_to_usermode()
{
  set_kernel_stack(4000 + KERNEL_STACK_SIZE);

  jump_usermode();
}

void test_user_function()
{
  print("Hello From User Mode!\n");
}
