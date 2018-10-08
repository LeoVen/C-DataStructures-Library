/**
 * @file Stack.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_STACK_H
#define C_DATASTRUCTURES_LIBRARY_STACK_H

#include "Core.h"

typedef struct Stack
{
    size_t height;        /*!< @c Stack height */
    struct StackBox_s *top; /*!< @c StackBox on top */
} Stack_t, *Stack;

Status stk_init(Stack *stk);

Status stk_push(Stack stk, int value);

Status stk_pop(Stack stk, int *result);

Status stk_display(Stack stk);

Status stk_display_array(Stack stk);

Status stk_display_raw(Stack stk);

Status stk_delete(Stack *stk);

Status stk_erase(Stack *stk);

int stk_peek(Stack stk);

size_t stk_height(Stack stk);

bool stk_empty(Stack stk);

Status stk_copy(Stack stk, Stack *result);

#endif //C_DATASTRUCTURES_LIBRARY_STACK_H
