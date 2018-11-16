/**
 * @file Stack.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#include "Stack.h"

/// This is a linked list implementation of a \c Stack with FILO (First-in
/// Last-out) or LIFO (Last-in First-out) operations, so the first item added
/// is the last one to be removed. It is implemented as a SinglyLinkedList but
///  with restricted operations to preserve the FILO (LIFO) order of elements.
/// The function \c stk_push() is equivalent to \c sll_insert_head() and the
/// function \c stk_pop() is equivalent to \c sll_remove_head(). Removal and
/// insertions are O(1). Both push and pop functions operate on what would be
/// the \c head of a SinglyLinkedList.
///
/// To initialize the stack use stk_init().
///
/// \b Advantages over \c StackArray
/// - Indefinitely grows
/// - No need to reallocate buffers
///
/// \b Drawbacks
/// - No random access
/// - More memory usage as in every node there is a pointer to the next node
///
/// \b Functions
/// Located in file Stack.c
struct Stack_s
{
    /// \brief Current amount of elements in the \c Stack.
    ///
    /// Current amount of elements in the \c Stack.
    index_t height;

    /// \brief Stack height limit.
    ///
    /// If it is set to 0 or a negative value then the stack has no limit to
    /// its height. Otherwise it won't be able to have more elements than the
    /// specified value. The stack is always initialized with no restrictions
    /// to its length, that is, \c limit equals 0. The user won't be able to
    /// limit the stack height if it already has more elements than the
    /// specified limit.
    index_t limit;

    /// \brief The element at the top of the \c Stack.
    ///
    /// The element at the top of \c Stack. Push and Pop operate relative to
    /// this pointer. It points to \c NULL if the \c Stack is empty.
    struct StackNode_s *top;

    /// \brief Comparator function.
    ///
    /// A function that compares one element with another that returns an int
    /// with the following rules:
    ///
    /// - <code>[ > 0 ]</code> if first element is greater than the second;
    /// - <code>[ < 0 ]</code> if second element is greater than the first;
    /// - <code>[ 0 ]</code> if elements are equal.
    stk_compare_f d_compare;

    /// \brief Copy function.
    ///
    /// A function that returns an exact copy of an element.
    stk_copy_f d_copy;

    /// \brief Display function.
    ///
    /// A function that displays an element in the console. Useful for
    /// debugging.
    stk_display_f d_display;

    /// \brief Deallocator function.
    ///
    /// A function that completely frees an element from memory.
    stk_free_f d_free;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    index_t version_id;
};

/// \brief A Stack_s node.
///
/// Implementation detail. This is a singly-linked node. It has one data member
/// and one pointer to the next node or \c NULL if it is the bottom-most node.
struct StackNode_s
{
    /// \brief Data pointer.
    ///
    /// Points to node's data. The data needs to be dynamically allocated.
    void *data;

    /// \brief Node underneath the current node.
    ///
    /// Node underneath the current node or \c NULL if it is the bottom-most
    /// node.
    struct StackNode_s *below;
};

/// \brief A type for a stack node.
///
/// Defines a type to a <code> struct StackNode_s </code>.
typedef struct StackNode_s StackNode_t;

/// \brief A pointer type for a stack node.
///
/// Defines a pointer type to a <code> struct StackNode_s </code>.
typedef struct StackNode_s *StackNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status stk_make_node(StackNode *node, void *element);

static Status stk_free_node(StackNode *node, stk_free_f free_f);

static Status stk_free_node_shallow(StackNode *node);

static Status stk_insert_bottom(Stack stack, StackNode ref, void *element);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes the Stack_s structure.
///
/// Initializes a new Stack_s with no default functions. Be sure to initialize
/// them when needed or some functions might return with an error.
///
/// \param[in,out] stack The stack to be initialized.
///
/// \return DS_ERR_ALLOC if stack allocation failed.
/// \return DS_OK if all operations were successful.
Status stk_init(Stack *stack)
{
    (*stack) = malloc(sizeof(Stack_t));

    if (!(*stack))
        return DS_ERR_ALLOC;

    (*stack)->top = NULL;
    (*stack)->height = 0;
    (*stack)->limit = 0;
    (*stack)->version_id = 0;

    (*stack)->d_compare = NULL;
    (*stack)->d_copy = NULL;
    (*stack)->d_display = NULL;
    (*stack)->d_free = NULL;

    return DS_OK;
}

/// \brief Creates a Stack_s.
///
/// This function creates a Stack_s setting all of its default functions.
///
/// \param[in,out] stack The stack to be initialized.
/// \param[in] compare_f A function that compares two elements.
/// \param[in] copy_f A function that makes an exact copy of an element.
/// \param[in] display_f A function that displays in the console an element.
/// \param[in] free_f A function that completely frees from memory an element.
///
/// \return DS_ERR_ALLOC if stack allocation failed.
/// \return DS_OK if all operations were successful.
Status stk_create(Stack *stack, stk_compare_f compare_f, stk_copy_f copy_f,
        stk_display_f display_f, stk_free_f free_f)
{
    (*stack) = malloc(sizeof(Stack_t));

    if (!(*stack))
        return DS_ERR_ALLOC;

    (*stack)->top = NULL;
    (*stack)->height = 0;
    (*stack)->limit = 0;
    (*stack)->version_id = 0;

    (*stack)->d_compare = compare_f;
    (*stack)->d_copy = copy_f;
    (*stack)->d_display = display_f;
    (*stack)->d_free = free_f;

    return DS_OK;
}

/// \brief Frees from memory a Stack_s and all its elements.
///
/// Iterates through every node of the stack and frees them from memory along
/// with its data using the stack default free function. Then the Stack
/// structure is deallocated and set to \c NULL.
///
/// \param[in,out] stack The stack to be freed from memory.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_free(Stack *stack)
{
    if ((*stack) == NULL)
        return DS_ERR_INVALID_OPERATION;

    if ((*stack)->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    StackNode prev = (*stack)->top;

    Status st;

    while ((*stack)->top != NULL)
    {
        (*stack)->top = (*stack)->top->below;

        st = stk_free_node(&prev, (*stack)->d_free);

        if (st != DS_OK)
            return st;

        prev = (*stack)->top;
    }

    free(*stack);

    (*stack) = NULL;

    return DS_OK;
}

Status stk_free_shallow(Stack *stack)
{
    if ((*stack) == NULL)
        return DS_ERR_INVALID_OPERATION;

    StackNode prev = (*stack)->top;

    Status st;

    while ((*stack)->top != NULL)
    {
        (*stack)->top = (*stack)->top->below;

        st = stk_free_node_shallow(&prev);

        if (st != DS_OK)
            return st;

        prev = (*stack)->top;
    }

    free(*stack);

    (*stack) = NULL;

    return DS_OK;
}

/// This function sets the stack to its initial state, erasing all of its data
/// and re-initializing the structure. It is equivalent to calling stk_free()
/// and then stk_init().
///
/// \param stack The stack to be erased.
///
/// \return DS_ERR_ALLOC if stack allocation failed.
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_erase(Stack *stack)
{
    if ((*stack) == NULL)
        return DS_ERR_NULL_POINTER;

    Stack new_stack;

    Status st = stk_create(&new_stack, (*stack)->d_compare, (*stack)->d_copy,
                           (*stack)->d_display, (*stack)->d_free);

    if (st != DS_OK)
        return st;

    st = stk_free(stack);

    if (st != DS_OK)
    {
        free(new_stack);

        return st;
    }

    *stack = new_stack;

    return DS_OK;
}

/// \brief Sets the default compare function.
///
/// Use this function to set a default compare function. It needs to comply
/// with the stk_free_f specifications.
///
/// \param[in] stack Stack_s to set the default compare function.
/// \param[in] function An stk_compare_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the stack references to \c NULL.
/// \return DS_OK if all operations are successful.
Status stk_set_func_compare(Stack stack, stk_compare_f function)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    stack->d_compare = function;

    return DS_OK;
}

/// \brief Sets the default copy function.
///
/// Use this function to set a default compare function. It needs to comply
/// with the stk_copy_f specifications.
///
/// \param[in] stack Stack_s to set the default copy function.
/// \param[in] function An stk_copy_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the stack references to \c NULL.
/// \return DS_OK if all operations are successful.
Status stk_set_func_copy(Stack stack, stk_copy_f function)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    stack->d_copy = function;

    return DS_OK;
}

/// \brief Sets the default display function
///
/// Use this function to set a default display function. It needs to comply
/// with the stk_display_f specifications. Useful for debugging.
///
/// \param[in] stack Stack_s to set the default display function.
/// \param[in] function An stk_display_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the stack references to \c NULL.
/// \return DS_OK if all operations are successful.
Status stk_set_func_display(Stack stack, stk_display_f function)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    stack->d_display = function;

    return DS_OK;
}

/// \brief Sets the default free function
///
/// Use this function to set a default free function. It needs to comply
/// with the stk_free_f specifications.
///
/// \param[in] stack Stack_s to set the default free function.
/// \param[in] function An stk_free_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the stack references to \c NULL.
/// \return DS_OK if all operations are successful.
Status stk_set_func_free(Stack stack, stk_free_f function)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    stack->d_free = function;

    return DS_OK;
}

/// \brief Sets a limit to the specified Stack_s's length.
///
/// Limit's the Stack_s's length. You can only set a limit greater or
/// equal to the stack's current length and greater than 0. To remove this
/// limitation simply set the limit to 0 or less.
///
/// \param[in] stack Stack_s reference.
/// \param[in] limit Maximum stack height.
///
/// \return DS_ERR_INVALID_OPERATION if the limitation is less than the stack's
/// current length.
/// \return DS_ERR_NULL_POINTER if the stack references to \c NULL.
/// \return DS_OK if all operations are successful.
Status stk_set_limit(Stack stack, index_t limit)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    // The new limit can't be lower than the stack's current length.
    if (stack->height > limit && limit > 0)
        return DS_ERR_INVALID_OPERATION;

    stack->limit = limit;

    return DS_OK;
}

index_t stk_height(Stack stack)
{
    if (stack == NULL)
        return -1;

    return stack->height;
}

index_t stk_limit(Stack stack)
{
    if (stack == NULL)
        return -1;

    return stack->limit;
}

/// Inserts an element into the specified stack. The element is added relative
/// to the \c rear pointer.
///
/// \param[in] stack The stack where the element is to be inserted.
/// \param[in] element The element to be inserted on the stack.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (more than 0) and the stack height
/// reached the specified limit.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_push(Stack stack, void *element)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_full(stack))
        return DS_ERR_FULL;

    StackNode box;

    Status st = stk_make_node(&box, element);

    if (st != DS_OK)
        return st;

    box->below = stack->top;
    stack->top = box;

    stack->height++;
    stack->version_id++;

    return DS_OK;
}

/// \brief Pops an element from the specified Stack_s.
///
/// Removes and retrieves an element from the specified stack relative to the
/// top pointer.
///
/// \param[in] stack The stack where the element is to be removed from.
///
/// \return NULL if the stack references \c NULL or if the stack is empty.
/// \return The top element.
void *stk_pop(Stack stack)
{
    if (stack == NULL)
        return NULL;

    if (stk_empty(stack))
        return NULL;

    StackNode box = stack->top;

    stack->top = stack->top->below;

    void *result = box->data;

    Status st = stk_free_node_shallow(&box);

    if (st != DS_OK)
        return NULL;

    stack->height--;
    stack->version_id++;

    return result;
}

/// Alias to stk_push().
///
/// \param[in] stack The stack where the element is to be inserted.
/// \param[in] element The element to be inserted on the stack.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (more than 0) and the stack height
/// reached the specified limit.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
///
/// \see stk_push()
Status stk_insert(Stack stack, void *element)
{
    return stk_push(stack, element);
}

/// \brief Works like stk_pop().
///
/// Alias to stk_pop() but result is in the arguments and returns a Status.
///
/// \param[in] stack The stack where the element is to be removed from.
/// \param[out] result The resulting element removed from the stack.
///
/// \return DS_ERR_INVALID_OPERATION if the stack is empty.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
///
/// \see stk_pop()
Status stk_remove(Stack stack, void **result)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_empty(stack))
        return DS_ERR_INVALID_OPERATION;

    StackNode box = stack->top;

    stack->top = stack->top->below;

    *result = box->data;

    Status st = stk_free_node_shallow(&box);

    if (st != DS_OK)
        return st;

    stack->height--;
    stack->version_id++;

    return DS_OK;
}

/// \brief Removes but does not retrieves the top element.
///
/// This function simply removes the top element of the stack without
/// retrieving the element. For this you must have set a default free function.
///
/// \param[in] stack The stack where the element is to be removed from.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_INVALID_OPERATION if the stack is empty.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_slice(Stack stack)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_empty(stack))
        return DS_ERR_INVALID_OPERATION;

    if (stack->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    StackNode box = stack->top;

    stack->top = stack->top->below;

    Status st = stk_free_node(&box, stack->d_free);

    if (st != DS_OK)
        return st;

    stack->height--;
    stack->version_id++;

    return DS_OK;
}

bool stk_full(Stack stack)
{
    return stack->limit > 0 && stack->height >= stack->limit;
}

bool stk_empty(Stack stack)
{
    return stack->height == 0;
}

void *stk_peek(Stack stack)
{
    if (stack == NULL)
        return NULL;

    if (stk_empty(stack))
        return NULL;

    return stack->top->data;
}

Status stk_copy(Stack stack, Stack *result)
{
    *result = NULL;

    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    if (stack->d_copy == NULL || stack->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st = stk_create(result, stack->d_compare, stack->d_copy,
            stack->d_display, stack->d_free);

    if (st != DS_OK)
        return st;

    (*result)->limit = stack->limit;

    StackNode prev = NULL, copy, scan = stack->top;

    void *elem;

    while (scan != NULL)
    {
        elem = stack->d_copy(scan->data);

        st = stk_make_node(&copy, elem);

        if (st != DS_OK)
        {
            stk_free_node(&copy, stack->d_free);

            return st;
        }

        if (prev == NULL)
        {
            prev = copy;

            (*result)->top = prev;
        }
        else
        {
            prev->below = copy;

            prev = prev->below;
        }

        scan = scan->below;

        copy = NULL;
    }

    (*result)->height = stack->height;

    return DS_OK;
}

/// Displays a \c Stack in the console.
///
/// \param stack The stack to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_display(Stack stack)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    if (stack->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (stk_empty(stack))
    {
        printf("\nStack\n[ empty]\n");

        return DS_OK;
    }

    StackNode scan = stack->top;

    printf("\nStack");

    while (scan != NULL)
    {
        printf("| ");

        stack->d_display(scan->data);

        printf(" |\n");

        scan = scan->below;
    }

    printf("\n");

    return DS_OK;
}

/// Displays a \c Stack in the console like an array with its values separated
/// by commas, delimited with brackets.
///
/// \param stack The stack to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_display_array(Stack stack)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    if (stack->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (stk_empty(stack))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    StackNode scan = stack->top;

    printf("\n[ ");

    while (scan->below != NULL)
    {
        stack->d_display(scan->data);

        printf(", ");

        scan = scan->below;
    }

    stack->d_display(scan->data);

    printf(" ]\n");

    return DS_OK;
}

/// Displays a \c Stack in the console with its values separated by spaces.
///
/// \param stack The stack to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_display_raw(Stack stack)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    if (stack->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    printf("\n");

    if (stk_empty(stack))
        return DS_OK;

    StackNode scan = stack->top;

    while (scan != NULL)
    {
        stack->d_display(scan->data);

        printf(" ");

        scan = scan->below;
    }

    printf("\n");

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status stk_make_node(StackNode *node, void *element)
{
    (*node) = malloc(sizeof(StackNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->below = NULL;
    (*node)->data = element;

    return DS_OK;
}

static Status stk_free_node(StackNode *node, stk_free_f free_f)
{
    if ((*node) == NULL)
        return DS_ERR_NULL_POINTER;

    free_f((*node)->data);

    free(*node);

    *node = NULL;

    return DS_OK;
}

static Status stk_free_node_shallow(StackNode *node)
{
    if (*node == NULL)
        return DS_ERR_NULL_POINTER;

    free(*node);

    *node = NULL;

    return DS_OK;
}

static Status stk_insert_bottom(Stack stack, StackNode ref, void *element)
{
    if (stack == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_full(stack))
        return DS_ERR_FULL;

    StackNode node;

    Status st = stk_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (stk_empty(stack) || ref == NULL)
    {
        stack->top = node;
    }
    else
    {
        ref->below = node;
    }

    (stack->height)++;

    return DS_OK;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

struct StackIterator_s
{
    struct Stack_s *target;
    struct StackNode_s *cursor;
    index_t target_it;
};
