/**
 * @file AssociativeList.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 28/12/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_ASSOCIATIVELIST_H
#define C_DATASTRUCTURES_LIBRARY_ASSOCIATIVELIST_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct AssociativeList_s
///
/// \brief A generic linked list of key-value pairs.
struct AssociativeList_s;

/// \brief A type for a an associative list.
///
/// A type for a <code> struct AssociativeList_s </code> so you don't have to
/// always write the full name of it.
typedef struct AssociativeList_s AssociativeList_t;

/// \brief A pointer type for an associative list.
///
/// A pointer type to <code> struct AssociativeList_s </code>. This typedef is
/// used to avoid having to declare every array as a pointer type since they
/// all must be dynamically allocated.
typedef struct AssociativeList_s *AssociativeList;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref ali_new
/// \brief Initializes a new AssociativeList_s.
AssociativeList_t *
ali_new(Interface_t *key_interface, Interface_t *value_interface,
        bool duplicate_keys);

/// \ref ali_free
/// \brief Frees from memory an AssociativeList_s and its element pairs.
void
ali_free(AssociativeList_t *list);

/// \ref ali_free_shallow
/// \brief Frees from memory a AssociativeList_s leaving its elements intact.
void
ali_free_shallow(AssociativeList_t *list);

/// \ref ali_erase
/// \brief Frees from memory all element pairs of an AssociativeList_t.
void
ali_erase(AssociativeList_t *list);

/// \ref ali_erase
/// \brief Frees from memory all nodes of an AssociativeList_t.
void
ali_erase_shallow(AssociativeList_t *list);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref ali_config
/// \brief Sets new interfaces for a target associative list.
void
ali_config(AssociativeList_t *list, Interface_t *key_interface,
           Interface_t *value_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref ali_length
/// \brief Returns the amount of elements in the specified associative list.
integer_t
ali_length(AssociativeList_t *list);

/// \ref ali_limit
/// \brief Returns the current set limit of the associative list.
integer_t
ali_limit(AssociativeList_t *list);

/// \ref ali_multiple_keys
/// \brief Returns true if the associative list is set to have multiple keys.
bool
ali_multiple_keys(AssociativeList_t *list);

/// \ref ali_get
/// \brief Returns the value associated with a key, or NULL if not found.
void *
ali_get(AssociativeList_t *list, void *key);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref ali_set_limit
/// \brief Sets a limit to the amount of key-value pairs in the list.
bool
ali_set_limit(AssociativeList_t *list, integer_t limit);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref ali_insert
/// \brief Inserts a new key mapped to a value in the list.
bool
ali_insert(AssociativeList_t *list, void *key, void *value);

/// \ref ali_remove
/// \brief Removes a given key from the list and retrieves its value.
bool
ali_remove(AssociativeList_t *list, void *key, void **value);

/// \ref ali_pop
/// \brief Removes a given key from the list and does not retrieves it.
bool
ali_pop(AssociativeList_t *list, void *key);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref ali_full
/// \brief Returns true if the specified associative list is full.
bool
ali_full(AssociativeList_t *list);

/// \ref ali_empty
/// \brief Returns true if the specified associative list is empty.
bool
ali_empty(AssociativeList_t *list);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref ali_contains_key
/// \brief Returns true if the associative list contains a given key.
bool
ali_contains_key(AssociativeList_t *list, void *key);

/// \ref ali_contains_value
/// \brief Returns true if the associative list contains a given value.
bool
ali_contains_value(AssociativeList_t *list, void *key);

/// \ref ali_to_arrays
/// \brief Creates two arrays with keys and values from an existing list.
bool
ali_to_arrays(AssociativeList_t *list, void ***K_array, void ***V_array);

/// \ref ali_from_arrays
/// \brief Creates an associative list from two arrays of keys and values.
bool
ali_from_arrays(AssociativeList_t **list, void **K_array, void **V_array,
        unsigned_t from_index, unsigned_t to_index);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref ali_display
/// \brief Displays in the console an associative list.
void
ali_display(AssociativeList_t *list);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo AssociativeListIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo AssociativeListWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_ASSOCIATIVELIST_H
