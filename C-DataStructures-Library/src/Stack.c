/**
 * @file Stack.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#include "Stack.h"

/// \brief A linked list implementation of a generic stack.
///
/// This is a linked list implementation of a Stack with FILO (First-in
/// Last-out) or LIFO (Last-in First-out) operations, so the first item added
/// is the last one to be removed. It is implemented as a SinglyLinkedList_s
/// but with restricted operations to preserve the FILO (LIFO) order of
/// elements. The function \c stk_push() is equivalent to \c sll_insert_head()
/// and the function \c stk_pop() is equivalent to \c sll_remove_head().
/// Removal and insertions are O(1). Both push and pop functions operate on
/// what would be the \c head of a SinglyLinkedList_s.
///
/// To initialize the stack use stk_init(). This only initializes the
/// structure. If you don't set the default functions later you won't be able
/// to do certain operations. If you want to initialize it completely, use
/// instead stk_create(). Here you must pass in default functions (compare,
/// copy, display and free) according with the specifications of each type of
/// function.
///
/// To insert elements in the stack use stk_push() or stk_insert() as an alias.
/// To remove an element you have three options: stk_pop() that will return
/// \c NULL if something goes wrong; stk_remove() that returns a Status and the
/// result is in the arguments; or stk_slice() that will simply remove the top
/// element without returning it, so this function needs a default free
/// function to work.
///
/// To delete a stack use stk_free(). This completely frees all elements and
/// sets the stack pointer to \c NULL. Note that if you haven't set a default
/// free function you won't be able to delete the stack or its elements. You
/// must set a free function that will be responsible for freeing from memory
/// all elements. You can also use stk_free_shallow() that will only free the
/// stack structure. If you simply want to erase all its contents use
/// stk_erase(). This will keep all default functions and all elements will be
/// removed from the stack and freed from memory.
///
/// The stack maintains a version id that keeps track of structural changes
/// done to the stack. This prevents any iterators from working the moment the
/// stack structure is changed. It works to prevent any undefined behaviour or
/// run-time errors.
///
/// \b Advantages over StackArray_s
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
    /// \brief Current amount of elements in the Stack_s.
    ///
    /// Current amount of elements in the Stack_s.
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
    /// The element at the top of Stack_s. stk_push() and stk_pop() operate
    /// relative to this pointer. It points to \c NULL if the stack is empty.
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
/// and one pointer to the next node or \c NULL if it is the bottommost node.
struct StackNode_s
{
    /// \brief Data pointer.
    ///
    /// Points to node's data. The data needs to be dynamically allocated.
    void *data;

    /// \brief Node underneath the current node.
    ///
    /// Node underneath the current node or \c NULL if it is the bottommost
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

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes a Stack_s structure.
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

/// \brief Frees from memory a Stack_s.
///
/// This function frees from memory all the stack's nodes without freeing its
/// data and then frees the stack's structure. The variable is then set to
/// \c NULL.
///
/// \param[in,out] stack Stack_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the stack references to \c NULL.
/// \return DS_OK if all operations are successful.
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

/// \brief Erases a Stack_s.
///
/// This function sets the stack to its initial state, erasing all of its data
/// and re-initializing the structure. It is equivalent to calling stk_free()
/// and then stk_init().
///
/// \param[in,out] stack The stack to be erased.
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

/// \brief Returns the Stack_s's height.
///
/// Returns the total elements in the Stack_s or -1 if the stack references to
/// \c NULL.
///
/// \param[in] stack Stack_s reference.
///
/// \return -1 if the stack reference is \c NULL.
/// \return The stack's height.
index_t stk_height(Stack stack)
{
    if (stack == NULL)
        return -1;

    return stack->height;
}

/// \brief Returns the Stack_s's limit.
///
/// Returns the stack limit or -1 if the stack references to \c NULL.
///
/// \param[in] stack Stack_s reference.
///
/// \return -1 if the stack reference is \c NULL.
/// \return The stack's limit.
index_t stk_limit(Stack stack)
{
    if (stack == NULL)
        return -1;

    return stack->limit;
}

/// \brief Inserts an element to the specified Stack_s.
///
/// Inserts an element into the specified Stack_s. The element at the top of
/// of the stack.
///
/// \param[in] stack Stack_s reference where the element is to be inserted.
/// \param[in] element Element to be inserted in the stack.
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

/// \brief Alias to stk_push().
///
/// An alias to stk_push().
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

/// \brief Removes an element from the specified Stack_s.
///
/// Works like stk_pop() but the resulting element is in the arguments and
/// the function returns a Status.
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

/// \brief Checks if the specified Stack_s is full.
///
/// Returns true if the stack is full or false otherwise. The stack can only be
/// full if its limit is set to a value higher than 0 and respecting all rules
/// from stk_set_limit().
///
/// \warning This function does not checks for \c NULL references and is prone
/// to provoke run-time errors if not used correctly.
///
/// \param[in] stack Stack_s reference.
///
/// \return True if the stack is full.
/// \return False if the stack is not full.
bool stk_full(Stack stack)
{
    return stack->limit > 0 && stack->height >= stack->limit;
}

/// \brief Checks if specified Stack_s stack is empty.
///
/// Returns true if the stack is empty or false otherwise. The stack is empty
/// when its length is 0. If every function works properly there is no need to
/// check if the top pointer is \c NULL or not. The length variable already
/// tracks it.
///
/// \warning This function does not checks for \c NULL references and is prone
/// to provoke run-time errors if not used correctly.
///
/// \param[in] stack Stack_s reference.
///
/// \return True if the stack is empty.
/// \return False if the stack is not empty.
bool stk_empty(Stack stack)
{
    return stack->height == 0;
}

/// \brief Peeks the topmost element.
///
/// Returns the pointer to the topmost element. Please use it as a read-only.
///
/// \param[in] stack Stack_s reference.
///
/// \return \c NULL if either the stack parameter references to \c NULL or if
/// the stack is empty.
/// \return The topmost element.
void *stk_peek(Stack stack)
{
    if (stack == NULL)
        return NULL;

    if (stk_empty(stack))
        return NULL;

    return stack->top->data;
}

/// \brief Checks if a given element is present in the specified Stack_s.
///
/// Returns true if the element is present in the stack, otherwise false.
///
/// \warning This function does not checks for \c NULL references for either
/// the stack parameter or if the default compare function is set.
///
/// \param[in] stack Stack_s reference.
/// \param[in] key Key to be matched.
///
/// \return True if the element is present in the stack.
/// \return False if the element is not present in the stack.
bool stk_contains(Stack stack, void *key)
{
    StackNode scan = stack->top;

    while (scan != NULL)
    {
        if (stack->d_compare(scan->data, key) == 0)
            return true;

        scan = scan->below;
    }

    return false;
}

/// \brief Makes a copy of the specified Stack_s.
///
/// Makes an exact copy of a stack using a default copy function for each
/// element. It appends every node at the end of the stack until the end of the
/// source stack.
///
/// \param[in] stack Stack_s to be copied.
/// \param[out] result Resulting copy.
///
/// \return DS_ERR_INCOMPLETE_TYPE if either a default copy or a default free
/// functions are not set.
/// \return DS_ERR_NULL_POINTER if stack references to \c NULL.
/// \return DS_OK if all operations are successful.
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

/// \brief Stacks one stack at the top of the other.
///
/// Stacks the \c stack2 on top of the \c stack1, emptying the \c stack2. Both
/// stacks need to have been initialized. If the stack1 is empty it will
/// receive all elements contained in the stack2, otherwise the bottommost
/// node of stack2 will point to the top node of stack1. If both stacks are
/// empty nothing will happen.
///
/// \param[in] stack1 Stack to receive elements.
/// \param[in] stack2 Stack where the elements are going to be taken from.
///
/// \return DS_ERR_NULL_POINTER if either stack1 or stack2 are referencing to
/// \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_stack(Stack stack1, Stack stack2)
{
    if (stack1 == NULL || stack2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_empty(stack2))
        return DS_OK;

    if (stk_empty(stack1))
    {
        stack1->top = stack2->top;
        stack1->height = stack2->height;

        stack2->top = NULL;
        stack2->height = 0;
    }
    else
    {
        StackNode scan = stack2->top;

        while (scan->below !=  NULL)
            scan = scan->below;

        // stack2 goes on top of stack1
        scan->below = stack1->top;

        stack1->top = stack2->top;
        stack1->height += stack2->height;

        stack2->top = NULL;
        stack2->height = 0;
    }

    stack1->version_id++;
    stack2->version_id++;

    return DS_OK;
}

/// \brief Displays a Stack_s in the console.
///
/// Displays a Stack_s in the console with each element in one line separated
/// by vertical lines or pipes to represent a stack of boxes.
///
/// \param[in] stack The stack to be displayed in the console.
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

    printf("\nStack\n");

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

/// \brief Displays a Stack_s in the console like an array.
///
/// Displays a Stack_s in the console like an array with its values separated
/// by commas, delimited with brackets. The first element is the topmost
/// element in the stack and the last element is the bottommost element.
///
/// \param[in] stack The stack to be displayed in the console.
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

/// \brief Displays a Stack_s in the console.
///
/// Displays a Stack_s in the console with its values separated by spaces.
///
/// \param[in] stack The stack to be displayed in the console.
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

// Makes a node.
static Status stk_make_node(StackNode *node, void *element)
{
    (*node) = malloc(sizeof(StackNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->below = NULL;
    (*node)->data = element;

    return DS_OK;
}

// Frees a node and its element.
static Status stk_free_node(StackNode *node, stk_free_f free_f)
{
    if ((*node) == NULL)
        return DS_ERR_NULL_POINTER;

    free_f((*node)->data);

    free(*node);

    *node = NULL;

    return DS_OK;
}

// Frees a node but does not frees the element.
static Status stk_free_node_shallow(StackNode *node)
{
    if (*node == NULL)
        return DS_ERR_NULL_POINTER;

    free(*node);

    *node = NULL;

    return DS_OK;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \brief An iterator for a Stack_s.
///
/// This iterator is a forward-only iterator.
struct StackIterator_s
{
    /// \brief Target Stack_s.
    ///
    /// Target Stack_s. The iterator might need to use some information
    /// provided by the stack or change some of its data members.
    struct Stack_s *target;

    /// \brief Current element.
    ///
    /// Points to the current node. The iterator is always initialized with the
    /// cursor pointing to the start (top) of the stack.
    struct StackNode_s *cursor;

    /// \brief Target version ID.
    ///
    /// When the iterator is initialized it stores the version_id of the target
    /// structure. This is kept to prevent iteration on the target structure
    /// that may have been modified and thus causing undefined behaviours or
    /// run-time crashes.
    index_t target_id;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool stk_iter_target_modified(StackIterator iter);

static bool stk_iter_invalid_state(StackIterator iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

Status stk_iter_init(StackIterator *iter, Stack target)
{
    *iter = malloc(sizeof(StackIterator_t));

    if (!(*iter))
        return DS_ERR_ALLOC;

    (*iter)->target = target;
    (*iter)->target_id = target->version_id;
    (*iter)->cursor = target->top;

    return DS_OK;
}

Status stk_iter_retarget(StackIterator *iter, Stack target)
{
    Status st = stk_iter_free(iter);

    if (st != DS_OK)
        return st;

    st = stk_iter_init(iter, target);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

Status stk_iter_free(StackIterator *iter)
{
    if (*iter == NULL)
        return DS_ERR_NULL_POINTER;

    free(*iter);

    *iter = NULL;

    return DS_OK;
}

Status stk_iter_next(StackIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (stk_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!stk_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->cursor->below;

    return DS_OK;
}

Status stk_iter_to_top(StackIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (stk_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!stk_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->target->top;

    return DS_OK;
}

bool stk_iter_has_next(StackIterator iter)
{
    return iter->cursor->below != NULL;
}

Status stk_iter_get(StackIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (stk_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (iter->target->d_copy == NULL || iter->target->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    *result = iter->target->d_copy(iter->cursor->data);

    return DS_OK;
}

Status stk_iter_set(StackIterator iter, void *element)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (stk_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (iter->target->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    iter->target->d_free(iter->cursor->data);

    iter->cursor->data = element;

    return DS_OK;
}

Status stk_iter_insert(StackIterator iter, void *element)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (stk_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    StackNode node;

    Status st = stk_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (stk_empty(iter->target))
    {
        iter->target->top = node;

        iter->cursor = node;
    }
    else
    {
        node->below = iter->cursor->below;

        iter->cursor->below = node;
    }

    iter->target->height++;

    iter->target->version_id++;
    iter->target_id++;

    return DS_OK;
}

Status stk_iter_remove(StackIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (stk_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (stk_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!stk_iter_has_next(iter))
        return DS_ERR_ITER;

    Status st;

    if (stk_height(iter->target) == 1)
    {
        *result = iter->cursor->data;

        st = stk_free_node_shallow(&(iter->cursor));

        if (st != DS_OK)
            return st;

        iter->cursor = NULL;

        iter->target->top = NULL;
    }
    else
    {
        StackNode node = iter->cursor->below;

        *result = node->data;

        iter->cursor->below = node->below;

        st = stk_free_node_shallow(&node);

        if (st != DS_OK)
            return st;
    }

    iter->target->height--;

    iter->target->version_id++;
    iter->target_id++;

    return DS_OK;
}

void *stk_iter_peek_next(StackIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (stk_iter_invalid_state(iter))
        return NULL;

    if (stk_iter_target_modified(iter))
        return NULL;

    if (!stk_iter_has_next(iter))
        return NULL;

    return iter->cursor->below->data;
}

void *stk_iter_peek(StackIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (stk_iter_invalid_state(iter))
        return NULL;

    if (stk_iter_target_modified(iter))
        return NULL;

    return iter->cursor->data;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool stk_iter_target_modified(StackIterator iter)
{
    return iter->target_id != iter->target->version_id;
}

static bool stk_iter_invalid_state(StackIterator iter)
{
    return iter->cursor == NULL || iter->target == NULL;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///