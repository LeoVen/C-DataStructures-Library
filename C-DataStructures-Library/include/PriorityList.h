/**
 * @file PriorityList.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 28/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_PRIORITYLIST_H
#define C_DATASTRUCTURES_LIBRARY_PRIORITYLIST_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct PriorityList_s
/// \brief A generic, linked list based priority queue.
struct PriorityList_s;

/// \ref PriorityList_t
/// \brief A type for a priority list.
///
/// A type for a <code> struct PriorityList_s </code> so you don't have to
/// always write the full name of it.
typedef struct PriorityList_s PriorityList_t;

/// \ref PriorityList
/// \brief A pointer type for a priority list.
///
/// Defines a pointer type to <code> struct PriorityList_s </code>. This
/// typedef is used to avoid having to declare every priority list as a pointer type
/// since they all must be dynamically allocated.
typedef struct PriorityList_s *PriorityList;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref pli_new
/// \brief Initializes a new priority list.
PriorityList_t *
pli_new(Interface_t *interface);

/// \ref pli_free
/// \brief Frees from memory a PriorityList_s and its elements.
void
pli_free(PriorityList_t *plist);

/// \ref pli_free_shallow
/// \brief Frees from memory a PriorityList_s leaving its elements intact.
void
pli_free_shallow(PriorityList_t *plist);

/// \ref pli_erase
/// \brief Resets the PriorityList_t freeing all its elements.
void
pli_erase(PriorityList_t *plist);

/// \ref pli_erase_shallow
/// \brief Resets the PriorityList_t without freeing its elements.
void
pli_erase_shallow(PriorityList_t *plist);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref pli_config
/// \brief Sets a new interface for the target priority list.
void
pli_config(PriorityList_t *plist, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref pli_count
/// \brief Returns the amount of elements in the specified priority list.
integer_t
pli_count(PriorityList_t *plist);

/// \ref pli_limit
/// \brief Returns the current limit of the priority list.
integer_t
pli_limit(PriorityList_t *plist);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref pli_set_limit
/// \brief Sets a limit to the priority list's length.
bool
pli_set_limit(PriorityList_t *plist, integer_t limit);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref pli_insert
/// \brief Inserts an element in the priority list.
bool
pli_insert(PriorityList_t *plist, void *element);

/// \ref pli_remove
/// \brief Removes the highest priority element from the priority list.
bool
pli_remove(PriorityList_t *plist, void **result);

/// \ref pli_peek
/// \brief Return the highest priority element of the priority list.
void *
pli_peek(PriorityList_t *plist);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref pli_empty
/// \brief Returns true if the priority list is empty, otherwise false.
bool
pli_empty(PriorityList_t *plist);

/// \ref pli_full
/// \brief Returns true if the priority list is full, otherwise false.
bool
pli_full(PriorityList_t *plist);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref pli_contains
/// \brief Returns true if a given element is present in the list.
bool
pli_contains(PriorityList_t *plist, void *key);

/// \ref pli_copy
/// \brief Makes a copy of an existing priority list.
PriorityList_t *
pli_copy(PriorityList_t *plist);

/// \ref pli_copy_shallow
/// \brief Makes a shallow copy of an existing priority list.
PriorityList_t *
pli_copy_shallow(PriorityList_t *plist);

/// \ref pli_compare
/// \brief Compares two priority lists according to \ref compare_f.
int
pli_compare(PriorityList_t *plist1, PriorityList_t *plist2);

/// \ref pli_merge
/// \brief Merges two priority lists.
bool
pli_merge(PriorityList_t *plist1, PriorityList_t *plist2);

/// \ref pli_to_array
/// \brief Makes a copy of the specified list as a C array.
void **
pli_to_array(PriorityList_t *plist, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref pli_display
/// \brief Displays a PriorityList_s in the console.
void
pli_display(PriorityList_t *plist, int display_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo PriorityListIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo PriorityListWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_PRIORITYLIST_H
