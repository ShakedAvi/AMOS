#ifndef USERMODE_H
#define USERMODE_H

#include "screen.h"
#include "paging.h"
#include "task.h"

extern void jump_usermode();
void jump_to_usermode();
void test_user_function();

#endif
