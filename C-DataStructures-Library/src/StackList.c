/**
 * @file StackList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#include "StackList.h"

/// This is a linked list implementation of a Stack with FILO (First-in
/// Last-out) or LIFO (Last-in First-out) operations, so the first item added
/// is the last one to be removed. It is implemented as a SinglyLinkedList_s
/// but with restricted operations to preserve the FILO (LIFO) order of
/// elements. The function \c stl_push() is equivalent to \c sll_insert_head()
/// and the function \c stl_pop() is equivalent to \c sll_remove_head().
/// Removal and insertions are O(1). Both push and pop functions operate on
/// what would be the \c head of a SinglyLinkedList_s.
///
/// To initialize the stack use stl_init(). This only initializes the
/// structure. If you don't set the default functions later you won't be able
/// to do certain operations. If you want to initialize it completely, use
/// instead stl_create(). Here you must pass in default functions (compare,
/// copy, display and free) according with the specifications of each type of
/// function.
///
/// To insert elements in the stack use stl_push() or stl_insert() as an alias.
/// To remove an element you have three options: stl_pop() that will return
/// \c NULL if something goes wrong; stl_remove() that returns a Status and the
/// result is in the arguments; or stl_slice() that will simply remove the top
/// element without returning it, so this function needs a default free
/// function to work.
///
/// To delete a stack use stl_free(). This completely frees all elements and
/// sets the stack pointer to \c NULL. Note that if you haven't set a default
/// free function you won't be able to delete the stack or its elements. You
/// must set a free function that will be responsible for freeing from memory
/// all elements. You can also use stl_free_shallow() that will only free the
/// stack structure. If you simply want to erase all its contents use
/// stl_erase(). This will keep all default functions and all elements will be
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
/// Located in the file StackList.c
struct StackList_s
{
    /// \brief Current amount of elements in the StackList_s.
    ///
    /// Current amount of elements in the StackList_s.
    integer_t count;

    /// \brief StackList count limit.
    ///
    /// If it is set to 0 or a negative value then the stack has no limit to
    /// its count. Otherwise it won't be able to have more elements than the
    /// specified value. The stack is always initialized with no restrictions
    /// to its length, that is, \c limit equals 0. The user won't be able to
    /// limit the stack count if it already has more elements than the
    /// specified limit.
    integer_t limit;

    /// \brief The element at the top of the \c StackList.
    ///
    /// The element at the top of StackList_s. stl_push() and stl_pop() operate
    /// relative to this pointer. It points to \c NULL if the stack is empty.
    struct StackListNode_s *top;

    /// \brief StackList_s interface.
    ///
    /// An interface is like a table that has function pointers for functions
    /// that will manipulate a desired data type.
    struct Interface_s *interface;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

/// This can be used together with VLAs to allocate the structure on the stack
/// instead of a heap allocation.
const unsigned_t stl_size = sizeof(StackList_t);

/// \brief A StackList_s node.
///
/// Implementation detail. This is a singly-linked node. It has one data member
/// and one pointer to the next node or \c NULL if it is the bottommost node.
struct StackListNode_s
{
    /// \brief Data pointer.
    ///
    /// Points to node's data. The data needs to be dynamically allocated.
    void *data;

    /// \brief Node underneath the current node.
    ///
    /// Node underneath the current node or \c NULL if it is the bottommost
    /// node.
    struct StackListNode_s *below;
};

/// \brief A type for a stack node.
///
/// Defines a type to a <code> struct StackListNode_s </code>.
typedef struct StackListNode_s StackListNode_t;

/// \brief A pointer type for a stack node.
///
/// Defines a pointer type to a <code> struct StackListNode_s </code>.
typedef struct StackListNode_s *StackListNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static StackListNode_t *
stl_new_node(void *element);

static void
stl_free_node(StackListNode_t *node, free_f function);

static void
stl_free_node_shallow(StackListNode_t *node);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a new StackList_s with a given interface that contains
/// functions to handle any user-defined type.
/// \par Interface Requirements
/// - None
///
/// \param[in] interface An interface defining all necessary functions for the
/// stack to operate.
///
/// \return A new StackList_s or NULL if allocation failed.
StackList_t *
stl_new(Interface_t *interface)
{
    StackList_t *stack = malloc(sizeof(StackList_t));

    if (!stack)
        return NULL;

    stack->count = 0;
    stack->limit = 0;
    stack->version_id = 0;
    stack->top = NULL;

    stack->interface = interface;

    return stack;
}

/// Initializes a new StackList_s allocated on the stack, given an interface
/// that contains functions to handle any user-defined type.
/// \par Interface Requirements
/// - None
///
/// \param[in,out] stack The stack allocated on the stack to be initialized.
/// \param[in] interface An interface defining all necessary functions for the
/// stack to operate.
///
/// \return True if all operations were successful.
bool
stl_init(StackList_t *stack, Interface_t *interface)
{
    stack->count = 0;
    stack->limit = 0;
    stack->version_id = 0;
    stack->top = NULL;
    stack->interface = interface;

    return true;
}

/// Iterates through every node of the stack and frees them from memory along
/// with its data using the stack interface's free function. Then the stack
/// struct is freed.
/// \par Interface Requirements
/// - free
///
/// \param[in] stack The stack to be freed from memory.
void
stl_free(StackList_t *stack)
{
    StackListNode_t *prev = stack->top;

    while (stack->top != NULL)
    {
        stack->top = stack->top->below;

        stl_free_node(prev, stack->interface->free);

        prev = stack->top;
    }

    free(stack);
}

/// This function frees from memory all the stack's nodes without freeing its
/// data and then frees the stack struct.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack StackList_s to be freed from memory.
void
stl_free_shallow(StackList_t *stack)
{
    StackListNode_t *prev = stack->top;

    while (stack->top != NULL)
    {
        stack->top = stack->top->below;

        stl_free_node_shallow(prev);

        prev = stack->top;
    }

    free(stack);
}

/// This function will reset the StackList_s, freeing all of its elements,
/// keeping the structure intact including its original interface.
/// \par Interface Requirements
/// - free
///
/// \param[in] stack The stack to have its elements erased.
void
stl_erase(StackList_t *stack)
{
    StackListNode_t *prev = stack->top;

    while (stack->top != NULL)
    {
        stack->top = stack->top->below;

        stl_free_node(prev, stack->interface->free);

        prev = stack->top;
    }

    stack->count = 0;
    stack->version_id++;
}

/// This function will reset the StackList_s, freeing all of its nodes but not
/// its elements, keeping the structure intact including its original
/// interface.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The stack to have its nodes erased.
void
stl_erase_shallow(StackList_t *stack)
{
    StackListNode_t *prev = stack->top;

    while (stack->top != NULL)
    {
        stack->top = stack->top->below;

        stl_free_node_shallow(prev);

        prev = stack->top;
    }

    stack->count = 0;
    stack->version_id++;
}

/// Sets a new interface for the specified StackList_s.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack StackList_s to change the interface.
/// \param[in] new_interface New interface for the specified structure.
void
stl_config(StackList_t *stack, Interface_t *new_interface)
{
    stack->interface = new_interface;
}

/// Returns the current amount of elements in the specified stack.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack StackList_s reference.
///
/// \return The stack total amount of elements.
integer_t
stl_count(StackList_t *stack)
{
    return stack->count;
}

/// Returns the current stack limit.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack StackList_s reference.
///
/// \return The current stack limit.
integer_t
stl_limit(StackList_t *stack)
{
    return stack->limit;
}

/// Limit's the StackList_s's length. You can only set a limit greater or equal
/// to the stack's current length and greater than 0. To remove this limitation
/// simply set the limit to 0 or less.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack StackList_s reference.
/// \param[in] limit New stack limit.
///
/// \return True if a new limit was set. False if the new limit is lower than
/// the current amount of elements in the stack.
bool
stl_set_limit(StackList_t *stack, integer_t limit)
{
    // The new limit can't be lower than the stack's current length.
    if (stack->count > limit && limit > 0)
        return false;

    stack->limit = limit;

    return true;
}

/// Inserts an element at the top of the specified stack.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack StackList_s reference where the element is to be inserted.
/// \param[in] element Element to be inserted onto the stack.
///
/// \return True if the element was successfully added to the stack or false if
/// the stack reached its limit size or node allocation failed.
bool
stl_push(StackList_t *stack, void *element)
{
    if (stl_full(stack))
        return false;

    StackListNode_t *node = stl_new_node(element);

    if (!node)
        return false;

    node->below = stack->top;
    stack->top = node;

    stack->count++;
    stack->version_id++;

    return true;
}

/// Removes and retrieves an element from the specified stack relative to the
/// top pointer.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The stack where the element is to be removed from.
/// \param[out] result The resulting element removed from the stack.
///
/// \return True if an element was removed from the stack or false if the stack
/// is empty.
bool
stl_pop(StackList_t *stack, void **result)
{
    *result = NULL;

    if (stl_empty(stack))
        return false;

    StackListNode_t *node = stack->top;
    stack->top = stack->top->below;

    *result = node->data;

    stl_free_node_shallow(node);

    stack->count--;
    stack->version_id++;

    return true;
}

/// Returns the element at the top of the stack or NULL if the stack is empty.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack StackList_s reference.
///
/// \return NULL if the stack is empty or the element at the top of the stack.
void *
stl_peek(StackList_t *stack)
{
    if (stl_empty(stack))
        return NULL;

    return stack->top->data;
}

/// Returns true if the stack is empty or false otherwise. The stack is empty
/// when its length is 0. If every function works properly there is no need to
/// check if the top pointer is \c NULL or not. The length variable already
/// tracks it.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack StackList_s reference.
///
/// \return True if the stack is empty, otherwise false.
bool
stl_empty(StackList_t *stack)
{
    return stack->count == 0;
}

/// Returns true if the stack is full or false otherwise. The stack can only be
/// full if its limit is set to a value higher than 0 and respecting all rules
/// from stl_set_limit().
/// \par Interface Requirements
/// - None
///
/// \param[in] stack StackList_s reference.
///
/// \return True if the amount of elements in the stack have reached a limit.
bool
stl_full(StackList_t *stack)
{
    return stack->limit > 0 && stack->count >= stack->limit;
}

/// Returns true if the specified size will fit in the stack before it reaches
/// its limit (if the limit is set).
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The target stack.
/// \param[in] size The specified size.
///
/// \return True if a given size fits inside the stack if it has a limit.
bool
stl_fits(StackList_t *stack, unsigned_t size)
{
    if (stack->limit <= 0)
        return true;

    return (stack->count + size) <= stack->limit;
}

/// Returns true if the element is present in the stack, otherwise false.
/// \par Interface Requirements
/// - compare
///
/// \param[in] stack StackList_s reference.
/// \param[in] key Key to be matched.
///
/// \return True if the element is present in the stack, otherwise false.
bool
stl_contains(StackList_t *stack, void *key)
{
    StackListNode_t *scan = stack->top;

    while (scan != NULL)
    {
        if (stack->interface->compare(scan->data, key) == 0)
            return true;

        scan = scan->below;
    }

    return false;
}

/// Returns a copy of the specified StackList_s with the same interface. All
/// elements are copied using the stack interface's copy function.
/// \par Interface Requirements
/// - copy
/// - free
///
/// \param[in] stack The stack to be copied.
///
/// \return NULL if allocation failed or a copy of the specified stack.
StackList_t *
stl_copy(StackList_t *stack)
{
    StackList_t *result = stl_new(stack->interface);

    if (!result)
        return NULL;

    result->limit = stack->limit;

    // scan -> goes through the original stack
    // copy -> current element being copied
    // prev -> copy's previous node to make the link
    StackListNode_t *prev = NULL, *copy = NULL, *scan = stack->top;

    while (scan != NULL)
    {
        void *element = stack->interface->copy(scan->data);
        copy = stl_new_node(element);

        if (!copy)
        {
            stack->interface->free(element);
            return false;
        }

        if (prev == NULL)
        {
            prev = copy;
            result->top = prev;
        }
        else
        {
            prev->below = copy;
            prev = prev->below;
        }

        scan = scan->below;
    }

    result->count = stack->count;

    return result;
}

/// Returns a shallow copy of the specified StackList_s with the same
/// interface.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The stack to be copied.
///
/// \return NULL if allocation failed or a shallow copy of the specified stack.
StackList_t *
stl_copy_shallow(StackList_t *stack)
{
    StackList_t *result = stl_new(stack->interface);

    if (!result)
        return NULL;

    result->limit = stack->limit;

    // scan -> goes through the original stack
    // copy -> current element being copied
    // prev -> copy's previous node to make the link
    StackListNode_t *prev = NULL, *copy, *scan = stack->top;

    while (scan != NULL)
    {
        copy = stl_new_node(scan->data);

        if (!copy)
        {
            stl_free_node(copy, stack->interface->free);
            return false;
        }

        if (prev == NULL)
        {
            prev = copy;
            result->top = prev;
        }
        else
        {
            prev->below = copy;
            prev = prev->below;
        }

        scan = scan->below;
    }

    result->count = stack->count;

    return result;
}

/// Makes a comparison between two stacks element by element. If one stack has
/// less elements than the other, the comparison of elements will go up until
/// one stack reaches its limit. If all elements are the same until then, the
/// tie breaker goes to their size. If it is also the same, then both stacks
/// are equal.
/// \par Interface Requirements
/// - compare
///
/// \param[in] stack1 A target stack to be compared.
/// \param[in] stack2 A target stack to be compared.
///
/// \return An int according to \ref compare_f.
int
stl_compare(StackList_t *stack1, StackList_t *stack2)
{
    StackListNode_t *scan1 = stack1->top, *scan2 = stack2->top;

    int comparison = 0;
    while (scan1 != NULL && scan2 != NULL)
    {
        comparison = stack1->interface->compare(scan1->data, scan2->data);
        if (comparison > 0)
            return 1;
        else if (comparison < 0)
            return -1;

        scan1 = scan1->below;
        scan2 = scan2->below;
    }

    // So far all elements were the same
    if (stack1->count > stack2->count)
        return 1;
    else if (stack1->count < stack2->count)
        return -1;

    return 0;
}

/// Stacks the \c stack2 on top of the \c stack1, emptying the \c stack2. Both
/// stacks need to have been initialized. If the stack1 is empty it will
/// receive all elements contained in the stack2, otherwise the bottommost
/// node of stack2 will point to the top node of stack1. If both stacks are
/// empty nothing will happen.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack1 Stack to receive elements.
/// \param[in] stack2 Stack where the elements are going to be taken from.
///
/// \return True if all operations were successful, otherwise false.
bool
stl_stack(StackList_t *stack1, StackList_t *stack2)
{
    if (stl_empty(stack2))
        return true;

    if (stl_empty(stack1))
    {
        stack1->top = stack2->top;
        stack1->count = stack2->count;

        stack2->top = NULL;
        stack2->count = 0;
    }
    else
    {
        StackListNode_t *scan = stack2->top;

        while (scan->below !=  NULL)
            scan = scan->below;

        // stack2 goes on top of stack1
        scan->below = stack1->top;

        stack1->top = stack2->top;
        stack1->count += stack2->count;

        stack2->top = NULL;
        stack2->count = 0;
    }

    stack1->version_id++;
    stack2->version_id++;

    return true;
}

/// Makes a copy of all the elements in the stack to a C array starting from
/// the top element.
/// \par Interface Requirements
/// - copy
///
/// \param[in] stack The stack to be copied to the array.
/// \param[out] length The resulting array's length.
///
/// \return The resulting array or NULL if the stack is empty or the array
/// allocation failed.
void **
stl_to_array(StackList_t *stack, integer_t *length)
{
    *length = 0;

    if (stl_empty(stack))
        return NULL;

    void **array = malloc(sizeof(void*) * (size_t)stack->count);

    if (!array)
        return NULL;

    StackListNode_t *scan = stack->top;

    integer_t i = 0;
    while (scan != NULL)
    {
        array[i++] = stack->interface->copy(scan->data);
        scan = scan->below;
    }

    *length = stack->count;

    return array;
}

/// Displays a StackList_s in the console starting from the top element. There
/// are currently four modes:
/// - -1 Displays each element separated by newline;
/// -  0 Displays each element like a linked list;
/// -  1 Displays each element separated by a space;
/// - Any other number defaults to the array representation.
/// \par Interface Requirements
/// - display
///
/// \param[in] stack The stack to be displayed in the console.
/// \param[in] display_mode How the stack is to be displayed in the console.
void
stl_display(StackList_t *stack, int display_mode)
{
    if (stl_empty(stack))
    {
        printf("\nStackList\n[ empty ]\n");
        return;
    }

    StackListNode_t *scan = stack->top;

    switch (display_mode)
    {
        case -1:
            printf("\nStackList\n");
            while (scan != NULL)
            {
                stack->interface->display(scan->data);
                printf("\n");
                scan = scan->below;
            }
            break;
        case 0:
            printf("\nStackList\nTop -> ");
            while (scan->below != NULL)
            {
                stack->interface->display(scan->data);
                printf(" -> ");
                scan = scan->below;
            }
            stack->interface->display(scan->data);
            printf(" NULL\n");
            break;
        case 1:
            printf("\nStackList\n");
            while (scan != NULL)
            {
                stack->interface->display(scan->data);
                printf(" ");
                scan = scan->below;
            }
            printf("\n");
            break;
        default:
            printf("\nStackList\n[ ");
            while (scan->below != NULL)
            {
                stack->interface->display(scan->data);
                printf(", ");
                scan = scan->below;
            }
            stack->interface->display(scan->data);
            printf(" ]\n");
            break;
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static StackListNode_t *
stl_new_node(void *element)
{
    StackListNode_t *node = malloc(sizeof(StackListNode_t));

    if (!node)
        return NULL;

    node->below = NULL;
    node->data = element;

    return node;
}

static void
stl_free_node(StackListNode_t *node, free_f function)
{
    function(node->data);
    free(node);
}

static void
stl_free_node_shallow(StackListNode_t *node)
{
    free(node);
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// This iterator is a forward-only iterator. Its cursor is represented by a
/// pointer to one of the stack's node.
struct StackListIterator_s
{
    /// \brief Target StackList_s.
    ///
    /// Target StackList_s. The iterator might need to use some information
    /// provided by the stack or change some of its data members.
    struct StackList_s *target;

    /// \brief Current element.
    ///
    /// Points to the current node. The iterator is always initialized with the
    /// cursor pointing to the start (top) of the stack.
    struct StackListNode_s *cursor;

    /// \brief Target version ID.
    ///
    /// When the iterator is initialized it stores the version_id of the target
    /// structure. This is kept to prevent iteration on the target structure
    /// that may have been modified and thus causing undefined behaviours or
    /// run-time crashes.
    integer_t target_id;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool
stl_iter_target_modified(StackListIterator_t *iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///
/// \param[in] target
///
/// \return
StackListIterator_t *
stl_iter_new(StackList_t *target)
{
    if (stl_empty(target))
        return NULL;

    StackListIterator_t *iter = malloc(sizeof(StackListIterator_t));

    if (!iter)
        return NULL;

    iter->target = target;
    iter->target_id = target->version_id;
    iter->cursor = target->top;

    return iter;
}

///
/// \param[in] iter
/// \param[in] target
void
stl_iter_retarget(StackListIterator_t *iter, StackList_t *target)
{
    iter->target = target;
    iter->target_id = target->version_id;
}

///
/// \param[in] iter
void
stl_iter_free(StackListIterator_t *iter)
{
    free(iter);
}

///
/// \param[in] iter
///
/// \return
bool
stl_iter_next(StackListIterator_t *iter)
{
    if (stl_iter_target_modified(iter))
        return false;

    if (!stl_iter_has_next(iter))
        return false;

    iter->cursor = iter->cursor->below;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
stl_iter_to_top(StackListIterator_t *iter)
{
    if (stl_iter_target_modified(iter))
        return false;

    iter->cursor = iter->target->top;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
stl_iter_has_next(StackListIterator_t *iter)
{
    return iter->cursor->below != NULL;
}

///
/// \param[in] iter
/// \param[in] result
///
/// \return
bool
stl_iter_get(StackListIterator_t *iter, void **result)
{
    if (stl_iter_target_modified(iter))
        return false;

    *result = iter->cursor->data;

    return true;
}

///
/// \param[in] iter
/// \param[in] element
///
/// \return
bool
stl_iter_set(StackListIterator_t *iter, void *element)
{
    if (stl_iter_target_modified(iter))
        return false;

    iter->target->interface->free(iter->cursor->data);

    iter->cursor->data = element;

    return true;
}

///
/// \param[in] iter
///
/// \return
void *
stl_iter_peek_next(StackListIterator_t *iter)
{
    if (stl_iter_target_modified(iter))
        return NULL;

    if (!stl_iter_has_next(iter))
        return NULL;

    return iter->cursor->below->data;
}

///
/// \param[in] iter
///
/// \return
void *
stl_iter_peek(StackListIterator_t *iter)
{
    if (stl_iter_target_modified(iter))
        return NULL;

    return iter->cursor->data;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool
stl_iter_target_modified(StackListIterator_t *iter)
{
    return iter->target_id != iter->target->version_id;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo StackListWrapper
