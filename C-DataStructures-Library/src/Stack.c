/**
 * @file Stack.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#include "Stack.h"

// NOT EXPOSED API

typedef struct StackBox
{
    int data;               /*!< Node's data */
    struct StackBox *below; /*!< Pointer to the box underneath it */
} StackBox;

Status stk_make_box(StackBox **box, int value);
Status stk_delete_box(StackBox **box);

// END OF NOT EXPOSED API

Status stk_init(Stack **stk)
{
    (*stk) = malloc(sizeof(Stack));

    if (!(*stk))
        return DS_ERR_ALLOC;

    (*stk)->height = 0;
    (*stk)->top = NULL;

    return DS_OK;
}

Status stk_push(Stack *stk, int value)
{
    if (stk == NULL)
        return DS_ERR_NULL_POINTER;

    StackBox *box;

    Status st = stk_make_box(&box, value);

    if (st != DS_OK)
        return st;

    box->below = stk->top;
    stk->top = box;

    (stk->height)++;

    return DS_OK;
}

Status stk_pop(Stack *stk, int *result)
{
    if (stk == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_empty(stk))
        return DS_ERR_INVALID_OPERATION;

    StackBox *box = stk->top;

    stk->top = stk->top->below;

    *result = box->data;

    Status st = stk_delete_box(&box);

    if (st != DS_OK)
        return st;

    (stk->height)--;

    return DS_OK;
}

Status stk_display(Stack *stk)
{
    if (stk == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk->height == 0 || stk->top == NULL)
    {

        printf("\nStack\n[ empty]\n");

        return DS_OK;
    }

    StackBox *scan = stk->top;

    printf("\nStack");

    while (scan != NULL)
    {
        printf("\n|%10d |", scan->data);
        scan = scan->below;
    }

    printf("\n");

    return DS_OK;
}

Status stk_display_array(Stack *stk)
{
    if (stk == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_empty(stk))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    StackBox *scan = stk->top;

    printf("\n[ ");

    while (scan->below != NULL)
    {
        printf("%d, ", scan->data);

        scan = scan->below;
    }

    printf("%d ]\n", scan->data);

    return DS_OK;
}

Status stk_display_raw(Stack *stk)
{
    if (stk == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (stk_empty(stk))
        return DS_OK;

    StackBox *scan = stk->top;

    while (scan != NULL)
    {
        printf(" %d", scan->data);
        scan = scan->below;
    }
    printf("\n");

    return DS_OK;
}

Status stk_delete(Stack **stk)
{
    if ((*stk) == NULL)
        return DS_ERR_INVALID_OPERATION;

    StackBox *prev = (*stk)->top;

    Status st;

    while ((*stk)->top != NULL)
    {
        (*stk)->top = (*stk)->top->below;

        st = stk_delete_box(&prev);

        if (st != DS_OK)
            return st;

        prev = (*stk)->top;
    }

    free(*stk);

    (*stk) = NULL;

    return DS_OK;
}

Status stk_erase(Stack **stk)
{
    if ((*stk) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = stk_delete(stk);

    if (st != DS_OK)
        return st;

    st = stk_init(stk);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

int stk_peek(Stack *stk)
{
    if (stk == NULL)
        return 0;
    
    if (stk_empty(stk))
        return 0;

    return stk->top->data;
}

size_t stk_height(Stack *stk)
{
    if (stk == NULL)
        return 0;

    return stk->height;
}

bool stk_empty(Stack *stk)
{
    return (stk->top == NULL || stk->height == 0);
}

Status stk_copy(Stack *stk, Stack **result)
{
    *result = NULL;

    if (stk == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = stk_init(result);

    if (st != DS_OK)
        return st;

    if (stk_empty(stk))
        return DS_OK;

    Stack *temp;

    st = stk_init(&temp);

    if (st != DS_OK)
        return st;

    StackBox *scan = stk->top;

    while (scan != NULL)
    {
        st = stk_push(temp, scan->data);

        if (st != DS_OK)
            return st;

        scan = scan->below;
    }

    scan = temp->top;

    while (scan != NULL)
    {
        st = stk_push(*result, scan->data);

        if (st != DS_OK)
            return st;

        scan = scan->below;
    }

    st = stk_delete(&temp);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

// NOT EXPOSED API

Status stk_make_box(StackBox **box, int value)
{
    (*box) = malloc(sizeof(StackBox));

    if (!(*box))
        return DS_ERR_ALLOC;

    (*box)->below = NULL;
    (*box)->data = value;

    return DS_OK;
}

Status stk_delete_box(StackBox **box)
{
    free(*box);

    *box = NULL;

    return DS_OK;
}

// END OF NOT EXPOSED API