/**
 * @file Interface.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 10/12/2018
 */

#include "Interface.h"

/// Read the Interface_s documentation to better understand what each function
/// is responsible to. For the configurations you don't want to set or that are
/// not necessary for your needs pass in the parameter as NULL.
///
/// \param compare A compare function.
/// \param copy A copy function.
/// \param display A display function.
/// \param free A free function.
/// \param hash A hash function.
/// \param priority A priority function.
///
/// \return A new interface or NULL if allocation failed.
Interface_t *
interface_new(compare_f compare, copy_f copy, display_f display, free_f free,
              hash_f hash, priority_f priority)
{
    Interface interface = malloc(sizeof(Interface_t));

    if (!interface)
        return NULL;

    interface->compare = compare;
    interface->copy = copy;
    interface->display = display;
    interface->free = free;
    interface->hash = hash;
    interface->priority = priority;

    return interface;
}

/// This function differs from interface_new() in that it initializes an
/// interface allocated in the stack.
///
/// \param interface The interface to be initialized.
/// \param compare A compare function.
/// \param copy A copy function.
/// \param display A display function.
/// \param free A free function.
/// \param hash A hash function.
/// \param priority A priority function.
void
interface_init(Interface_t *interface, compare_f compare, copy_f copy,
               display_f display, free_f free, hash_f hash,
               priority_f priority)
{
    interface->compare = compare;
    interface->copy = copy;
    interface->display = display;
    interface->free = free;
    interface->hash = hash;
    interface->priority = priority;
}

/// Changes the configuration of an interface. Any NULL parameters are ignored
/// and the previous configuration is kept.
///
/// \param interface An interface to be changed.
/// \param compare A compare function.
/// \param copy A copy function.
/// \param display A display function.
/// \param free A free function.
/// \param hash A hash function.
/// \param priority A priority function.
void
interface_config(Interface_t *interface,
                 compare_f compare, copy_f copy, display_f display,
                 free_f free, hash_f hash, priority_f priority)
{
    if (compare)
        interface->compare = compare;
    if (copy)
        interface->copy = copy;
    if (display)
        interface->display = display;
    if (free)
        interface->free = free;
    if (hash)
        interface->hash = hash;
    if (priority)
        interface->priority = priority;
}

/// Frees from memory the specified interface.
///
/// \param interface The interface to be deallocated.
void
interface_free(Interface_t *interface)
{
    free(interface);
}