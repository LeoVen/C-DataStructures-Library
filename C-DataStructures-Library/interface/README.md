# How to use Interfaces

In C we don't have templates and the only way to support all possible data types is by using functions. Say I have a data type called `my_struct` and I want to insert it into a sorted list; in C we don't have operator overloading and to substitute this we must use functions, like a comparator function that receives two `my_struct` types and returns an `int` saying if the first argument is greater than the second. Following this example, below is a continuation of how to make an interface for any data type.

```c
// Let my_struct be a structure or a custom data type with the following
// functions:

// A function that compares two structures of my data type
int compare_my_struct(const void *param1, const void *param2);

// A function that makes an exact copy of my data type
my_struct *copy_my_struct(const void *param);

// A function to displays my custom data type in the console
void display_my_struct(const void *param);

// A function that completely frees from memory my custom data type
void free_my_struct(void *param);

// A function that returns a unique hash number for my custom data type
size_t hash_my_struct(const void *param);

// A function that compares the priority of two of my custom data type. Note
// that this function is different from compare_my_struct() because it will
// only compare the priority of both elements, and not if they are equal. For
// simple types like int, double, char, etc, this can coincide with their
// compare function
int priority_my_struct(const void *param1, const void *param2);

// in Main, to create an interface use:
Interface my_struct_interface = interface_new(compare_my_struct, copy_my_struct,
                                              display_my_struct, free_my_struct,
                                              hash_my_struct, priority_my_struct);

// Now you can use my_struct_interface in any data structures.
StackArray my_stack = sta_new(my_struct_interface);
QueueArray my_queue = qua_new(my_struct_interface);

// Note that some data structures don't require that you have all functions set.
// For example, a QueueArray can have an interface with only the following
// functions:
Interface my_queue_interface = interface_new(compare_my_struct, copy_my_struct,
                                             NULL, free_my_struct, NULL, NULL);

// Or if you are only going to use the I/O functions you can even set an
// interface with all NULL parameters or initialize the interface as NULL:
QueueArray queue = qua_new(NULL);

// For associative containers you must specify two interfaces
HashMap my_map = hmp_new(my_key_interface, my_value_interface);

// For some data structures need an interface with a compare function to insert
// elements like trees and sorted containers.

// Non-generic structures like a bit array do not depend on interfaces.
```