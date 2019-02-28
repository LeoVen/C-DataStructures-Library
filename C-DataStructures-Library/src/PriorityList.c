/**
 * @file PriorityList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 28/09/2018
 */

#include "PriorityList.h"

/// A PriorityList is a data structure that sorts its elements according to
/// their natural priority implemented by their interface.
struct PriorityList_s
{
    /// \brief Current amount of elements in the priority list.
    ///
    /// Current amount of elements in the priority list.
    integer_t count;

    /// \brief Priority list length limit.
    ///
    /// If it is set to 0 or a negative value then the priority list has no
    /// limit to its length. Otherwise it won't be able to have more elements
    /// than the specified value. The priority list is always initialized with
    /// no restrictions to its length, that is, \c limit equals 0. The user
    /// won't be able to limit the list length if it already has more elements
    /// than the specified limit.
    integer_t limit;

    /// \brief The front of the priority list.
    ///
    /// Elements with higher priority are removed from the front of the
    /// priority list.
    struct PriorityListNode_s *front;

    /// \brief PriorityList_s interface.
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

/// \brief A PriorityList_s node.
///
/// Implementation detail. This node is like a singly-linked list node with one
/// pointer to a nextious node that has a lower priority than this one, or NULL
/// if this is the node with the lowest priority in the list.
struct PriorityListNode_s
{
    /// \brief Node's data.
    ///
    /// A pointer to the node's data.
    void *data;

    /// \brief Previous element in the priority list.
    ///
    /// Points to the nextious element in the priority list or NULL if this
    /// element is the one with the lowest priority in the list.
    struct PriorityListNode_s *next;
};

/// \brief A type for a priority list node.
///
/// Defines a type to a <code> struct PriorityListNode_s </code>.
typedef struct PriorityListNode_s PriorityListNode_t;

/// \brief A pointer type for a priority list node.
///
/// Defines a pointer type to a <code> struct PriorityListNode_s </code>.
typedef struct PriorityListNode_s *PriorityListNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static PriorityListNode_t *
pli_new_node(void *element);

static void
pli_free_node(PriorityListNode_t *node, free_f function);

static void
pli_free_node_shallow(PriorityListNode_t *node);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///
/// \param[in] interface
///
/// \return
PriorityList_t *
pli_new(Interface_t *interface)
{
    PriorityList_t *plist = malloc(sizeof(PriorityList_t));

    if (!plist)
        return NULL;

    plist->count = 0;
    plist->limit = 0;
    plist->version_id = 0;

    plist->front = NULL;

    plist->interface = interface;

    return plist;
}

///
/// \param[in] plist
void
pli_free(PriorityList_t *plist)
{
    PriorityListNode_t *scan = plist->front;

    while (plist->front != NULL)
    {
        plist->front = plist->front->next;

        pli_free_node(scan, plist->interface->free);

        scan = plist->front;
    }

    free(plist);
}

///
/// \param[in] plist
void
pli_free_shallow(PriorityList_t *plist)
{
    PriorityListNode_t *scan = plist->front;

    while (plist->front != NULL)
    {
        plist->front = plist->front->next;

        pli_free_node_shallow(scan);

        scan = plist->front;
    }

    free(plist);
}

///
/// \param[in] plist
void
pli_erase(PriorityList_t *plist)
{
    PriorityListNode_t *scan = plist->front;

    while (plist->front != NULL)
    {
        plist->front = plist->front->next;

        pli_free_node(scan, plist->interface->free);

        scan = plist->front;
    }

    plist->count = 0;
    plist->version_id++;

    plist->front = NULL;
}

///
/// \param[in] plist
void
pli_erase_shallow(PriorityList_t *plist)
{
    PriorityListNode_t *scan = plist->front;

    while (plist->front != NULL)
    {
        plist->front = plist->front->next;

        pli_free_node_shallow(scan);

        scan = plist->front;
    }

    plist->count = 0;
    plist->version_id++;

    plist->front = NULL;
}

///
/// \param[in] plist
/// \param[in] new_interface
void
pli_config(PriorityList_t *plist, Interface_t *new_interface)
{
    plist->interface = new_interface;
}

///
/// \param[in] plist
///
/// \return
integer_t
pli_count(PriorityList_t *plist)
{
    return plist->count;
}

///
/// \param[in] plist
///
/// \return
integer_t
pli_limit(PriorityList_t *plist)
{
    return plist->limit;
}

///
/// \param[in] plist
/// \param[in] limit
///
/// \return
bool
pli_set_limit(PriorityList_t *plist, integer_t limit)
{
    // The new limit can't be lower than the list's current length.
    if (plist->count > limit && limit > 0)
        return false;

    plist->limit = limit;

    return true;
}

///
/// \param[in] plist
/// \param[in] element
///
/// \return
bool
pli_insert(PriorityList_t *plist, void *element)
{
    if (pli_full(plist))
        return false;

    PriorityListNode_t *node = pli_new_node(element);

    if (!node)
        return false;

    if (pli_empty(plist))
    {
        // Case for the first node
        plist->front = node;
    }
    else
    {
        if (plist->interface->priority(node->data, plist->front->data) > 0)
        {
            // Case for when the front pointer needs to be changed
            // The element has the highest priority in the list
            node->next = plist->front;

            plist->front = node;
        }
        else
        {
            PriorityListNode_t *scan = plist->front->next;
            PriorityListNode_t *before = plist->front;

            while (scan != NULL &&
                   plist->interface->priority(node->data, scan->data) <= 0)
            {
                before = scan;
                scan = scan->next;
            }

            if (scan == NULL)
            {
                // The element has the lowest priority in the list
                before->next = node;
            }
            else
            {
                // Adding to the middle of the list
                before->next = node;
                node->next = scan;
            }
        }
    }

    plist->count++;
    plist->version_id++;

    return true;
}

///
/// \param[in] plist
/// \param[in] result
///
/// \return
bool
pli_remove(PriorityList_t *plist, void **result)
{
    if (pli_empty(plist))
        return false;

    PriorityListNode_t *node = plist->front;
    *result = node->data;

    plist->front = plist->front->next;

    pli_free_node_shallow(node);

    plist->count--;
    plist->version_id++;

    if (pli_empty(plist))
        plist->front = NULL;

    return true;
}

///
/// \param[in] plist
///
/// \return
void *
pli_peek(PriorityList_t *plist)
{
    if (pli_empty(plist))
        return NULL;

    return plist->front->data;
}

///
/// \param[in] plist
///
/// \return
bool
pli_empty(PriorityList_t *plist)
{
    return plist->count == 0;
}

///
/// \param[in] plist
///
/// \return
bool
pli_full(PriorityList_t *plist)
{
    return plist->limit > 0 && plist->count >= plist->limit;
}

///
/// \param[in] plist
/// \param[in] key
///
/// \return
bool
pli_contains(PriorityList_t *plist, void *key)
{
    PriorityListNode_t *scan = plist->front;

    while (scan != NULL)
    {
        if (plist->interface->compare(scan->data, key) == 0)
            return true;

        scan = scan->next;
    }

    return false;
}

///
/// \param[in] plist
///
/// \return
PriorityList_t *
pli_copy(PriorityList_t *plist)
{
    PriorityList_t *result = pli_new(plist->interface);

    if (!result)
        return NULL;

    result->limit = plist->limit;

    // scan -> goes through the original stack
    // copy -> current element being copied
    // prev -> copy's previous node to make the link
    PriorityListNode_t *prev = NULL, *copy, *scan = plist->front;

    while (scan != NULL)
    {
        copy = pli_new_node(plist->interface->copy(scan->data));

        if (!copy)
        {
            pli_free_node(copy, plist->interface->free);
            return false;
        }

        if (prev == NULL)
        {
            prev = copy;
            result->front = prev;
        }
        else
        {
            prev->next = copy;
            prev = prev->next;
        }

        scan = scan->next;
    }

    result->count = plist->count;

    return result;
}

///
/// \param[in] plist
///
/// \return
PriorityList_t *
pli_copy_shallow(PriorityList_t *plist)
{
    PriorityList_t *result = pli_new(plist->interface);

    if (!result)
        return NULL;

    result->limit = plist->limit;

    // scan -> goes through the original stack
    // copy -> current element being copied
    // prev -> copy's previous node to make the link
    PriorityListNode_t *prev = NULL, *copy, *scan = plist->front;

    while (scan != NULL)
    {
        copy = pli_new_node(scan->data);

        if (!copy)
        {
            pli_free_node_shallow(copy);
            return false;
        }

        if (prev == NULL)
        {
            prev = copy;
            result->front = prev;
        }
        else
        {
            prev->next = copy;
            prev = prev->next;
        }

        scan = scan->next;
    }

    result->count = plist->count;

    return result;
}

///
/// \param[in] plist1
/// \param[in] plist2
///
/// \return
int
pli_compare(PriorityList_t *plist1, PriorityList_t *plist2)
{
    PriorityListNode_t *scan1 = plist1->front, *scan2 = plist2->front;

    int comparison = 0;
    while (scan1 != NULL && scan2 != NULL)
    {
        comparison = plist1->interface->compare(scan1->data, scan2->data);
        if (comparison > 0)
            return 1;
        else if (comparison < 0)
            return -1;

        scan1 = scan1->next;
        scan2 = scan2->next;
    }

    // So far all elements were the same
    if (plist1->count > plist2->count)
        return 1;
    else if (plist1->count < plist2->count)
        return -1;

    return 0;
}

/// Merges list2 into list1, emptying the second.
///
/// \warning Both lists must have the same interface and handling the same data
/// type, otherwise you'll be mixing elements into a list that doesn't know how
/// to handle it (probably crashing).
///
/// \param[in] plist1
/// \param[in] plist2
///
/// \return True if all elements were merged, otherwise false.
bool
pli_merge(PriorityList_t *plist1, PriorityList_t *plist2)
{
    void *result;
    while (!pli_empty(plist2))
    {
        if (!pli_remove(plist2, &result))
            return false;

        // May cause memory leak...
        if (!pli_insert(plist1, result))
            return false;
    }

    return true;
}

///
/// \param[in] plist
/// \param[in] length
///
/// \return
void **
pli_to_array(PriorityList_t *plist, integer_t *length)
{
    *length = 0;

    if (pli_empty(plist))
        return NULL;

    void **array = malloc(sizeof(void*) * (size_t)plist->count);

    if (!array)
        return NULL;

    PriorityListNode_t *scan = plist->front;

    integer_t i = 0;
    while (scan != NULL)
    {
        array[i++] = plist->interface->copy(scan->data);
        scan = scan->next;
    }

    *length = plist->count;

    return array;
}

///
/// \param[in] plist
/// \param[in] display_mode
void
pli_display(PriorityList_t *plist, int display_mode)
{
    if (pli_empty(plist))
    {
        printf("\nPriorityList\n[ empty ]\n");
        return;
    }

    PriorityListNode_t *scan = plist->front;

    switch (display_mode)
    {
        case -1:
            printf("\nPriorityList\n");
            while (scan != NULL)
            {
                plist->interface->display(scan->data);
                printf("\n");
                scan = scan->next;
            }
            break;
        case 0:
            printf("\nPriorityList\nHigh -> ");
            while (scan->next != NULL)
            {
                plist->interface->display(scan->data);
                printf(" -> ");
                scan = scan->next;
            }
            plist->interface->display(scan->data);
            printf(" Low\n");
            break;
        case 1:
            printf("\nPriorityList\n");
            while (scan != NULL)
            {
                plist->interface->display(scan->data);
                printf(" ");
                scan = scan->next;
            }
            printf("\n");
            break;
        default:
            printf("\nPriorityList\n[ ");
            while (scan->next != NULL)
            {
                plist->interface->display(scan->data);
                printf(", ");
            }
            plist->interface->display(scan->data);
            printf(" ]\n");
            break;
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static PriorityListNode_t *
pli_new_node(void *element)
{
    PriorityListNode_t *node = malloc(sizeof(PriorityListNode_t));

    if (!node)
        return NULL;

    node->data = element;
    node->next = NULL;

    return node;
}

static void
pli_free_node(PriorityListNode_t *node, free_f function)
{
    function(node->data);
    free(node);
}

static void
pli_free_node_shallow(PriorityListNode_t *node)
{
    free(node);
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo PriorityListIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo PriorityListWrapper
