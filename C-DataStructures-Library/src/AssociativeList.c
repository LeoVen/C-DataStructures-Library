/**
 * @file AssociativeList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 28/12/2018
 */

#include "AssociativeList.h"

/// The associative list is a singly-linked list with keys that are mapped to a
/// single value or possibly more. Each node contains both the key and the
/// value that the key is mapped to.
struct AssociativeList_s
{
    /// \brief List length.
    ///
    /// Current amount of keys in the associative list.
    integer_t length;

    /// \brief List length limit.
    ///
    /// If it is set to 0 or a negative value then the list has no limit to its
    /// length. Otherwise it won't be able to have more key-value pairs than
    /// the specified value. The list is always initialized with no
    /// restrictions to its length, that is, \c limit equals 0. The user won't
    /// be able to limit the list length if it already has more pairs than the
    /// specified limit.
    integer_t limit;

    /// \brief A flag if multiple keys are allowed.
    ///
    /// If set to true then the associative list is allowed to store a new node
    /// with an already existing key.
    bool duplicate_keys;

    /// \brief Points to the first Node on the list.
    ///
    /// Points to the first Node on the list or \c NULL if the list is empty.
    struct AssociativeListNode_s *head;

    /// \brief Points to the last Node on the list
    ///
    /// Points to the last Node on the list or \c NULL if the list is empty.
    struct AssociativeListNode_s *tail;

    /// \brief AssociativeList_s key interface.
    ///
    /// An interface is like a table that has function pointers for functions
    /// that will manipulate a desired data type. This interface is responsible
    /// for handling all necessary operations on the keys of this associative
    /// list.
    struct Interface_s *K_interface;

    /// \brief AssociativeList_s value interface.
    ///
    /// An interface is like a table that has function pointers for functions
    /// that will manipulate a desired data type. This interface is responsible
    /// for handling all necessary operations on the values of this associative
    /// list.
    struct Interface_s *V_interface;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

/// \brief An AssociativeList_s node.
///
/// Implementation detail. This is a singly-linked node. It has one key member,
/// one value member and one pointer to the next node or \c NULL if it is the
/// tail node.
struct AssociativeListNode_s
{
    /// \brief This node's key.
    ///
    /// Represents the key in this associative container.
    void *key;

    /// \brief This node's value.
    ///
    /// Represents the value in this associative container.
    void *value;

    /// \brief Next node on the list.
    ///
    /// Next node on the list or NULL if this is the last element.
    struct AssociativeListNode_s *next;
};

typedef struct AssociativeListNode_s AssociativeListNode_t;

typedef struct AssociativeListNode_s *AssociativeListNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static AssociativeListNode_t *
ali_new_node(void *key, void *value);

static void
ali_free_node(AssociativeListNode_t *node, free_f key_free,
              free_f value_free);

static void
ali_free_node_shallow(AssociativeListNode_t *node);

static AssociativeListNode_t *
ali_find(AssociativeList_t *list, AssociativeListNode_t **before, void *key);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Creates a new AssociativeList_s with two interfaces, one for the keys and
/// another the values; also a flag can be set to allow multiple values or not.
///
/// \param[in] key_interface Key interface.
/// \param[in] value_interface Value interface
/// \param[in] duplicate_keys A flag to allow multiple keys.
///
/// \return A new AssociativeList_s or NULL if allocation failed.
AssociativeList_t *
ali_new(Interface_t *key_interface, Interface_t *value_interface,
        bool duplicate_keys)
{
    AssociativeList_t *list = malloc(sizeof(AssociativeList_t));

    if (!list)
        return NULL;

    list->length = 0;
    list->limit = 0;
    list->version_id = 0;
    list->duplicate_keys = duplicate_keys;

    list->head = NULL;
    list->tail = NULL;

    list->K_interface = key_interface;
    list->V_interface = value_interface;

    return list;
}

///
/// \param[in] list
void
ali_free(AssociativeList_t *list)
{
    AssociativeListNode_t *prev = list->head;

    while (list->head != NULL)
    {
        list->head = list->head->next;

        ali_free_node(prev, list->K_interface->free, list->V_interface->free);

        prev = list->head;
    }

    free(list);
}

///
/// \param[in] list
void
ali_free_shallow(AssociativeList_t *list)
{
    AssociativeListNode_t *prev = list->head;

    while (list->head != NULL)
    {
        list->head = list->head->next;

        ali_free_node_shallow(prev);

        prev = list->head;
    }

    free(list);
}

///
/// \param[in] list
void
ali_erase(AssociativeList_t *list)
{
    AssociativeListNode_t *prev = list->head;

    while (list->head != NULL)
    {
        list->head = list->head->next;

        ali_free_node(prev, list->K_interface->free, list->V_interface->free);

        prev = list->head;
    }

    list->head = NULL;
    list->length = 0;
    list->version_id++;
}

///
/// \param[in] list
void
ali_erase_shallow(AssociativeList_t *list)
{
    AssociativeListNode_t *prev = list->head;

    while (list->head != NULL)
    {
        list->head = list->head->next;

        ali_free_node_shallow(prev);

        prev = list->head;
    }

    list->head = NULL;
    list->length = 0;
    list->version_id++;
}

///
/// \param[in] list
/// \param[in] key_interface
/// \param[in] value_interface
void
ali_config(AssociativeList_t *list, Interface_t *key_interface,
           Interface_t *value_interface)
{
    if (key_interface)
        list->K_interface = key_interface;

    if (value_interface)
        list->V_interface = value_interface;
}

///
/// \param[in] list
///
/// \return
integer_t
ali_length(AssociativeList_t *list)
{
    return list->length;
}

///
/// \param[in] list
///
/// \return
integer_t
ali_limit(AssociativeList_t *list)
{
    return list->limit;
}

///
/// \param[in] list
///
/// \return
bool
ali_duplicate_keys(AssociativeList_t *list)
{
    return list->duplicate_keys;
}

///
/// \param[in] list
/// \param[in] key
///
/// \return
void *
ali_get(AssociativeList_t *list, void *key)
{
    AssociativeListNode_t *_;
    AssociativeListNode_t *node = ali_find(list, &_, key);

    if (node == NULL)
        return NULL;

    return node->value;
}

///
/// \param[in] list
/// \param[in] limit
///
/// \return
bool
ali_set_limit(AssociativeList_t *list, integer_t limit)
{
    // The new limit can't be lower than the list's current length.
    if (list->length > limit && limit > 0)
        return false;

    list->limit = limit;

    return true;
}

///
/// \param[in] list
/// \param[in] key
/// \param[in] value
///
/// \return
bool
ali_insert(AssociativeList_t *list, void *key, void *value)
{
    if (ali_full(list))
        return false;

    // Duplicate keys not allowed, search if there is an existing one
    if (!list->duplicate_keys)
    {
        if (ali_contains_key(list, key))
            return false;
    }

    AssociativeListNode_t *node = ali_new_node(key, value);

    if (!node)
        return false;

    if (ali_empty(list))
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }

    list->length++;
    list->version_id++;

    return true;
}

///
/// \param[in] list
/// \param[in] key
/// \param[out] value
///
/// \return
bool
ali_remove(AssociativeList_t *list, void *key, void **value)
{
    *value = NULL;

    if (ali_empty(list))
        return false;

    AssociativeListNode_t *before;
    AssociativeListNode_t *node = ali_find(list, &before, key);

    // Not found
    if (node == NULL)
        return false;

    if (before == NULL)
    {
        // Removing head
        list->head = list->head->next;
    }
    else
    {
        before->next = node->next;
    }

    *value = node->value;

    list->K_interface->free(node->key);
    free(node);

    list->length--;
    list->version_id++;

    return true;
}

///
/// \param[in] list
/// \param[in] key
///
/// \return
bool
ali_pop(AssociativeList_t *list, void *key)
{
    AssociativeListNode_t *before;
    AssociativeListNode_t *node = ali_find(list, &before, key);

    // Not found
    if (node == NULL)
        return false;

    if (before == NULL)
    {
        // Removing head
        list->head = list->head->next;
    }
    else
    {
        before->next = node->next;
    }

    ali_free_node(node, list->K_interface->free, list->V_interface->free);

    list->length--;
    list->version_id++;

    return true;
}

///
/// \param[in] list
///
/// \return
bool
ali_full(AssociativeList_t *list)
{
    return list->limit > 0 && list->limit >= list->length;
}

///
/// \param[in] list
///
/// \return
bool
ali_empty(AssociativeList_t *list)
{
    return list->length == 0;
}

///
/// \param[in] list
/// \param[in] key
///
/// \return
bool
ali_contains_key(AssociativeList_t *list, void *key)
{
    AssociativeListNode_t *scan = list->head;

    while (scan != NULL)
    {
        if (list->K_interface->compare(key, scan->key) == 0)
            return true;

        scan = scan->next;
    }

    return false;
}

///
/// \param[in] list
/// \param[in] value
///
/// \return
bool
ali_contains_value(AssociativeList_t *list, void *value)
{
    AssociativeListNode_t *scan = list->head;

    while (scan != NULL)
    {
        if (list->V_interface->compare(value, scan->value) == 0)
            return true;

        scan = scan->next;
    }

    return false;
}

///
/// \param[in] list
/// \param[out] K_array
/// \param[out] V_array
///
/// \return
bool
ali_to_arrays(AssociativeList_t *list, void ***K_array, void ***V_array)
{
    /// \todo ali_to_arrays
    return  false;
}

///
/// \param[out] list
/// \param[in] K_array
/// \param[in] V_array
/// \param[in] from_index
/// \param[in] to_index
///
/// \return
bool
ali_from_arrays(AssociativeList_t **list, void **K_array, void **V_array,
               unsigned_t from_index, unsigned_t to_index)
{
    /// \todo ali_from_array
    return false;
}

///
/// \param[in] list
void
ali_display(AssociativeList_t *list)
{
    if (ali_empty(list))
    {
        printf("\nAssociativeList\n[ empty ]\n");
    }

    printf("\nAssociativeList\n");

    AssociativeListNode_t *scan = list->head;

    while (scan != NULL)
    {
        list->K_interface->display(scan->key);

        printf(" : ");

        list->V_interface->display(scan->value);

        printf("\n");

        scan = scan->next;
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static AssociativeListNode_t *
ali_new_node(void *key, void *value)
{
    AssociativeListNode_t *node = malloc(sizeof(AssociativeListNode_t));

    if (!node)
        return NULL;

    node->key = key;
    node->value = value;
    node->next = NULL;

    return node;
}

static void
ali_free_node(AssociativeListNode_t *node, free_f key_free,
              free_f value_free)
{
    key_free(node->key);
    value_free(node->value);

    free(node);
}

static void
ali_free_node_shallow(AssociativeListNode_t *node)
{
    free(node);
}

static AssociativeListNode_t *
ali_find(AssociativeList_t *list, AssociativeListNode_t **before, void *key)
{
    AssociativeListNode_t *scan = list->head;
    *before = NULL;

    while (scan != NULL)
    {
        if (list->K_interface->compare(key, scan->key) == 0)
            break;

        *before = scan;
        scan = scan->next;
    }

    return scan;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo AssociativeListIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo AssociativeListWrapper
