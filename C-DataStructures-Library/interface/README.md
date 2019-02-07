# How to use Interfaces

In C we don't have templates and the only way to support all possible data types is by using functions. Say I have a data type called `my_struct` and I want to insert it into a sorted list; in C we don't have operator overloading and to substitute this we must use functions, like a comparator function that receives two `my_struct` types and returns an `int` saying if the first argument is greater than the second. Following this example, below is a continuation of how to make an interface for any data type.

__Note that all functions operate on the type `void*` or `const void*` because this is the internal type of all data structures. You will have to cast all parameters every time. This is the downside of generics using `void*` in C.__

```c
// Let my_struct be a structure or a custom data type with the following
// functions:

// A function that compares two structures of my data type.
int compare_my_struct(const void *param1, const void *param2);

// A function that makes an exact copy of my data type.
void *copy_my_struct(const void *param);

// A function to displays my custom data type in the console.
void display_my_struct(const void *param);

// A function that completely frees from memory my custom data type.
void free_my_struct(void *param);

// A function that returns a unique hash number for my custom data type.
size_t hash_my_struct(const void *param);

// A function that compares the priority of two of my custom data type. Note
// that this function is different from compare_my_struct() because it will
// only compare the priority of both elements, and not if they are equal. For
// simple types like int, double, char, etc, this can coincide with their
// compare function.
int priority_my_struct(const void *param1, const void *param2);

```

Now to create an interface use:

```c
Interface_t *my_struct_interface = interface_new(compare_my_struct, copy_my_struct,
                                                 display_my_struct, free_my_struct,
                                                 hash_my_struct, priority_my_struct);
```

Now you can use `my_struct_interface` in any data structures.

```c
StackArray_t *my_stack = sta_new(my_struct_interface); // A stack of type my_struct
QueueArray_t *my_queue = qua_new(my_struct_interface); // A queue of type my_struct

// Note that some data structures don't require that you have all functions set.
// For example, a QueueArray_t can have an interface with only the following
// functions:
Interface my_queue_interface = interface_new(compare_my_struct, copy_my_struct,
                                             NULL, free_my_struct, NULL, NULL);

// Or if you are only going to use the I/O functions you can even set an
// interface with all NULL parameters or initialize the interface as NULL:
QueueArray queue = qua_new(NULL);
```

For associative containers you must specify two interfaces:

```c
HashMap_t *my_map = hmp_new(my_key_interface, my_value_interface);
```

Some data structures have mandatory interface functions to operate, like ordered data structures (sorted list, tree map, ect) or hashing data structures (hash map, multi hash map, etc).

Non-generic structures like a bit array do not depend on interfaces.
