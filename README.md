# C-DataStructures-Library

![license](https://img.shields.io/badge/license-GPL%203-blue.svg)
![build](https://travis-ci.org/LeoVen/C-DataStructures-Library.svg?branch=master)


The purpose of this library is to provide an easy to use, easy to understand and easy to reproduce implementation of data structures with full documentation and commented code when needed. This library has a focus on teaching purposes and the implementations are not high performance or platform/compiler independent.

## Planned Data Structures

|       Structure Name       |  Source Code   |    Iterator    |     Wrapper    |      Tests     |  Documentation |
| :------------------------- | :------------: | :------------: | :------------: | :------------: | :------------: |
| [Array][arr]               | `[##########]` | `[##########]` | `[__________]` | `[__________]` | `[#_________]` |
| [AssociativeList][asl]     | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [AVLTree][avl]             | `[##########]` | `[__________]` | `[__________]` | `[__________]` | `[#_________]` |
| [BinarySearchTree][bst]    | `[##########]` | `[__________]` | `[__________]` | `[__________]` | `[##________]` |
| [BinomialHeap][bnh]        | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [BitArray][bit]            | `[######____]` | `[__________]` | `[__________]` | `[###_______]` | `[####______]` |
| [BTree][btr]               | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [CircularLinkedList][cll]  | `[##########]` | `[##########]` | `[__________]` | `[#_________]` | `[#####_____]` |
| [CircularQueueList][cql]   | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [Deque][deq]               | `[##########]` | `[__________]` | `[__________]` | `[#_________]` | `[######____]` |
| [DequeArray][dqa]          | `[##########]` | `[__________]` | `[__________]` | `[##________]` | `[###_______]` |
| [Dictionary][dic]          | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [DoublyLinkedList][dll]    | `[########__]` | `[__________]` | `[__________]` | `[##________]` | `[#####_____]` |
| [DynamicArray][dar]        | `[##########]` | `[__________]` | `[__________]` | `[#_________]` | `[##________]` |
| [FibonacciHeap][fbh]       | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [HashMap][hmp]             | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [HashSet][hst]             | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [Heap][hep]                | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [MultiHashMap][mhm]        | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [MultiTreeMap][mtm]        | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [PriorityHeap][prh]        | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [PriorityQueue][prq]       | `[##########]` | `[__________]` | `[__________]` | `[__________]` | `[#_________]` |
| [Queue][que]               | `[##########]` | `[__________]` | `[__________]` | `[#_________]` | `[#######___]` |
| [QueueArray][qua]          | `[##########]` | `[__________]` | `[__________]` | `[##________]` | `[######____]` |
| [RadixTree][rdt]           | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [RedBlackTree][rbt]        | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [SinglyLinkedList][sll]    | `[#########_]` | `[__________]` | `[__________]` | `[#_________]` | `[######____]` |
| [SkipList][skp]            | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [SortedArray][sar]         | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [SortedList][sli]          | `[##########]` | `[##########]` | `[########__]` | `[###_______]` | `[##########]` |
| [SortedHashMap][shm]       | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [SortedHashSet][shs]       | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [SplayTree][spt]           | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [Stack][stk]               | `[##########]` | `[##########]` | `[__________]` | `[#_________]` | `[########__]` |
| [StackArray][sta]          | `[##########]` | `[__________]` | `[__________]` | `[#_________]` | `[######____]` |
| [TreeSet][trs]             | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [TreeMap][trm]             | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [Trie][tri]                | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| [UnrolledLinkedList][ull]  | `[__________]` | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|   __Completed__            |     __14__     |     __4__      |     __0__      |     __0__      |     __1__      |

## Summary
    
### Array

An array is an abstraction of a C array composed of a data buffer and a length variable. It is a static array, that is, it won't increase in size. Higher level languages provide a quick variable to get the array's length and this abstraction does exactly that. When you initialize the structure a length variable is stored and you can easily do a for loop like shown below.

```c
Array my_array;

// Initializes an array of 20 positions
arr_init(&my_array, 20);

// Many lines of code of code after...

int len = arr_length(my_array); // len = 20

for (int i = 0; i < len; i++)
{
    // Do something
    // Setting the position i with value (i * i)
    arr_set(my_array, i, i * i);
}
```

Note that the length variable is protected (implementation detail) and only accessible via a function. This is done so to prevent the user from changing the length variable of the array's struct, possibly causing run-time errors. Also access to the buffer is protected and you must use ```arr_set()``` to set a value in the array and ```arr_get()``` to get the value from the array.

### AssociativeList

Not implemented yet.

### AVLTree

An AVL tree is a type of a tree that has all the properties of a binary search tree with an additional one: at any given node, the absolute difference between heights of left sub-tree and right sub-tree cannot be greater than 1. This property is essential to keep the tree balanced.

Operations like insertion and removal are the same as a binary search tree differentiating only at the end of each function where the tree might need to be re-balanced. There are two rotations done to keep the tree balanced:

```
    T1, T2, T3 and T4 are subtrees.

    Right Rotation:

           Z                                      Y
          / \                                   /   \
         Y   T4      Right Rotate              X      Z
        / \          - - - - - - - - ->      /  \    /  \
       X   T3                               T1  T2  T3  T4
      / \
    T1   T2


    Left rotation:

      Z                                          Y
     /  \                                      /   \
    T1   Y           Left Rotate              Z      X
        /  \         - - - - - - - - ->      / \    / \
       T2   X                               T1  T2 T3  T4
           / \
         T3  T4
```

The AVL tree is more balanced than a red-black tree and has a guaranteed `O(log n)` search time. The drawbacks are that after inserting or removing an element the tree might need several rotations to satisfy the AVL tree properties.

### BinarySearchTree

A binary search tree is a node-based data structure which has the following properties:

* The left subtree of a node contains only nodes with keys lesser than the node’s key;
* The right subtree of a node contains only nodes with keys greater than the node’s key;
* The left and right subtree each must also be a binary search tree.

Also the tree usually does not have duplicate keys, but that can be arranged by using a count variable at each node or by chaining the repeated keys.

Some other properties:

* The maximum number of nodes at a level __L__ is given by <b>2^(L - 1)</b>;
* The maximum number of nodes of a tree with height __H__ is given by <b>2^h - 1</b>;
* A binary tree with __N__ nodes, the minimum number of levels is <b>Log<sub>2</sub>(N + 1)</b>;
* A binary tree with __N__ leaves nas at least <b>Log<sub>2</sub>(L) + 1</b>.

The problem with binary search trees is that they can get skewed. To solve this there are other variations like AVL trees and Red-black trees that can balance the distribution of nodes. If we add the numbers from 1 to 7 sequentially this is what would happen to a binary search tree versus an AVL tree:

```

    Binary Search Tree:                        AVL Tree:

    1                                                ( 4 )
     \                                              /     \
      2                                         ( 2 )     ( 6 )
       \                                        /   \     /    \
        3                                    ( 1 ) ( 3 )( 5 ) ( 7 )
         \
          4
           \
            5
             \
              6
               \
                7

```

This makes search as `O(N)` and is much slower than it should be so a balanced binary search tree is prefered for maintaining the shape of the tree to make operations run in faster times.

### BinomialHeap

Not implemented yet.

### BitArray

A bit array (bit set, bit map, bit string or bit vector) is a compacted array of bits represented by the bits in a word where you can individually set or clear each bit. It is very useful at implementing a Set of elements where:

* Union represented by the OR operator ( bit_OR() );
* Intersection represented by the AND operator ( bit_AND() );
* Symmetric difference represented by the XOR operator ( bit_XOR() ).

```
    A true value represented as a boolean
    ┌───────────────┐
    │       1       |
    └───────────────┘
    (8 bits used for one value)
    
    In a bit array we can represent 8 boolean values by mapping them for each bit
    ┌─┬─┬─┬─┬─┬─┬─┬─┐
    │1│1│0│1│0│1│0│1|
    └─┴─┴─┴─┴─┴─┴─┴─┘
    (8 bits used for 8 values)
    We effectively have the equivalent of an array of booleans but much more compressed    
```

### BTree

Not implemented yet.

### CircularLinkedList

A circular linked list is a doubly-linked list where its nodes wrap around making a circular structure. There are no `head` or `tail` pointers, only a cursor that can iterate through the circular list. One of its advantages are that there is no need to check for `NULL` pointers since the list technically has no 'head' or 'tail'.

```
                                      cursor
                                        │
                                        v
        ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐
    ┌─> │ G │ <-> │ E │ <-> │ C │ <-> │ A │ <-> │ B │ <-> │ D │ <-> │ F │ <-> │ H │ ───┐
    │   └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘    │
    └──────────────────────────────────────────────────────────────────────────────────┘
```

Due to its nature this structure comes with an iterator and there are 5 main input/output functions where they all operate relative to the list's cursor:

* cll_insert_after();
* cll_insert_before();
* cll_remove_after();
* cll_remove_current();
* cll_remove_before().

Being a doubly-linked list, all operations listed above take `O(1)`.

### CircularQueueList

Not implemented yet.

### Deque

A deque is a double-ended queue implemented as a doubly-linked list. Both operations `enqueue` and `dequeue` can be done to both ends of the queue.

The disadvantage of a Deque implemented as a linked list compared to a DequeArray that is implemented as a circular buffer is that it is very space inefficient where every node needs to store two pointers to its adjacent nodes; but it comes with an advantage where the linked list grows indefinitely and there is no need to shift elements or resize a buffer.

```
         ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐
NULL <-- │ G │ <-> │ E │ <-> │ C │ <-> │ A │ <-> │ B │ <-> │ D │ <-> │ F │ <-> │ H │ --> NULL
         └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘
           │                                                                     │
         front                                                                  rear
```

Its implementation is just like a doubly-linked list with limitations to input and output to both edges of the list.

### DequeArray

A deque array is the implementation of a deque using a circular buffer. It is very space efficient but unlike a Deque implemented as a linked list, a DequeArray will have to reallocate its buffer and shift its elements (if needed) whenever it reaches its maximum capacity. In a deque array, both front and rear pointers can wrap around the buffer making its implementation a bit more complex than a doubly-linked list.

```
    front and rear indexes have not wrapped around the buffer
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │   │   │ H | I | J | K | L | M |   |   |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
              │                   │
            front                rear
    
    rear index have wrapped around the buffer
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ P │ Q │ R |   |   |   |   | M | N | O |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
              │                   │
             rear               front
             
    front index have wrapped around the buffer
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ F │ G │ H |   |   |   |   | C | D | E |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
              │                   │
             rear               front
```

Note that when the `rear` index or the `front` index wraps around the buffer the configuration is the same.

### Dictionary

Not implemented yet.

### DoublyLinkedList

A doubly-linked list is a sequence of items, usually called nodes that are linked through pointers. It works like an array but has a structural difference where in an array the items are stored contiguously and in a linked list the items are stored in nodes that can be anywhere in memory. It is called doubly-linked because each node, besides having a data member and a pointer to the next node in the list, it also has a pointer to the previous node in the chained data structure.

```
         ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐
NULL <-- │ G │ <-> │ E │ <-> │ C │ <-> │ A │ <-> │ B │ <-> │ D │ <-> │ F │ <-> │ H │ --> NULL
         └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘
           │                                                                     │
          head                                                                  tail
```

Operations for inserting and removing elements at both ends take `O(1)`, and removing elements at the middle of the list take a maximum of `O(n / 2)` because if the index of the element is known we can know the best way to traverse the list to iterate the minimum amount of times, either starting from the start or the end of the list. This can be a big advantage over singly-linked lists.

### DynamicArray

A dynamic array automatically grows when its current capacity can't hold another item. When the array is full it is reallocated where a new buffer is allocated and then the original contents are copied to this new and bigger buffer. So in theory this array can take up as much space as needed if there is enough memory.

```
                                  can't add another element
                                              |
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ A │ B │ C | D | E | F | G | H | I | J |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
      │   │   │   │   │   │   │   │   │   │
      V   V   V   V   V   V   V   V   V   V
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ A │ B │ C | D | E | F | G | H | I | J |   |   |   |   |   |   |   |   |   |   |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
    new buffer gets reallocated, its original content is copied and the old buffer is freed
```

### FibonacciHeap

Not implemented yet.

### HashMap

Not implemented yet.

### HashSet

Not implemented yet.

### Heap

Not implemented yet.

### MultiHashMap

Not implemented yet.

### MultiTreeMap

Not implemented yet.

### PriorityHeap

Not implemented yet.

### PriorityQueue

The priority queue is a linked list implementation. It has a lot in common with a sorted list, but in this case the elements are sorted according to their priority.

### Queue

A queue is a FIFO (First-in First-out) or LILO (Last-in Last-out) abstract data type where the first element inserted is the first to be removed. This is a singly-linked implementation where enqueueing is equivalent to inserting and element at the tail of the list and dequeuing is equivalent to removing an element from the head of the list. This is done so both operations take `O(1)`, since the worst possible operation in a singly-linked list is removing the tail element where we would have to iterate over the entire list until the penultimate element.

 ```
    ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐
    │ A │ --> │ B │ --> │ C │ --> │ D │ --> │ E │ --> │ F │ --> │ G │ --> │ H │ --> NULL
    └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘
      │                                                                     │
    front                                                                  rear
 ```
 
 Elements are dequeued from the `front` pointer and enqueued from the `rear` pointer.

### QueueArray

A queue array is the implementation of a queue using a circular buffer. It is very space efficient but unlike a queue implemented as a linked list, a QueueArray will have to reallocate its buffer and shift its elements (if needed) whenever it reaches its maximum capacity. In a queue both front and rear indexes only go forward, that is, incremented by one when an operation is successful. If we enqueue an element the rear index goes up by one. If we dequeue an element the front index goes up by one.

```
    front and rear indexes have not wrapped around the buffer
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │   │   │ H | I | J | K | L | M |   |   |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
              │                   │
            front                rear
    
    rear index have wrapped around the buffer
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ P │ Q │ R |   |   |   |   | M | N | O |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
              │                   │
             rear               front
```

If the queue buffer is not full then the `rear` index will wrap around the buffer. When the buffer is full both front and rear are indexes are next to each other. After the buffer is reallocated there are two options to shift the elements. This is done to shift the least amount of elements as possible.

When the rear pointer is closer to the end it is much more efficient to simply send the last elements to the end of the buffer.

```
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ P │ Q │ R | S | T | U | V | M | N | O |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
                              │   │
                            rear front
    
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ P │ Q │ R | S | T | U | V |   |   |   |   |   |   |   |   |   |   | M | N | O |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
                              │                                           │
                             rear                                       front
```

But when the rear index is closer to the beginning of the buffer we can append the left portion to the right of the right-end portion.

```
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ P │ Q │ R | I | J | K | L | M | N | O |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
              │   │
            rear front
    
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │   │   │   | I | J | K | L | M | N | O | P | Q | R |   |   |   |   |   |   |   |   |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
                  │                                   │
                front                                rear
```

There is also a third option when there is no need to shift elements. This happens when you insert elements until the buffer is full and does no removes a single element. Everything stays as they should be. This can also be caused after removing elements if when the buffer fills up it coincides when the front index is `0` and the rear is `capacity - 1`.

```
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ F │ G │ H | I | J | K | L | M | N | O |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
      │                                   │
    front                                rear
    
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ F │ G │ H | I | J | K | L | M | N | O |   |   |   |   |   |   |   |   |   |   |   |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
      │                                   │
    front                                rear
```

### RadixTree

Not implemented yet.

### RedBlackTree

Not implemented yet.

### SinglyLinkedList

A singly-linked list is a sequence of items, usually called nodes that are linked through pointers. It works like an array but has a structural difference where in an array the items are stored contiguously and in a linked list the items are stored in nodes that can be anywhere in memory. It is called singly-linked because each node has only one pointer to the next node in the list.

```
    ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐
    │ G │ --> │ E │ --> │ C │ --> │ A │ --> │ B │ --> │ D │ --> │ F │ --> │ H │ --> NULL
    └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘
      │                                                                     │
     head                                                                  tail
```

Operations for inserting and removing elements at the head of the list take `O(1)`; to add an element at the tail of the list take `O(1)` but to remove it it is always `O(n - 1)` and this is a big disadvantage that singly-linked lists have over doubly-linked lists; removing elements at the middle of the list can take up t `O(n)` we must always start searching for an element at the head of the list.

### SkipList

Not implemented yet.

### SortedArray

Not implemented yet.

### SortedList

A sorted list is a generic doubly-linked sorted list. Its elements are sorted with a user-provided `compare` function. This list is very inefficient at inserting elements with the cost os keeping them sorted. A tree would keep the elements sorted much more easily but it would not be a linear structure. Being a linked list many operations will take `O(n)` but others like `sli_max()` and `sli_min()`, that retrieves the maximum and the minimum values, take only `O(1)` where in a balanced tree it would take `O(log n)`. Iteration is also simplified and you can also remove elements more easily.  

```c
SortedList slist;

sli_create(&slist, ASCENDING, compare_int, copy_int, display_int, free);

for (int i = 7; i >= 0; i--)
    sli_insert(slist, new_int(i));

sli_free(&slist); 
```

The above code would produce the following list.

```
         ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐
NULL <-- │ 0 │ <-> │ 1 │ <-> │ 2 │ <-> │ 3 │ <-> │ 4 │ <-> │ 5 │ <-> │ 6 │ <-> │ 7 │ --> NULL
         └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘
           │                                                                     │
          head                                                                  tail
```

### SortedHashMap

Not implemented yet.

### SortedHashSet

Not implemented yet.

### SplayTree

Not implemented yet.

### Stack

A stack is a FILO (First-in Last-out) or LIFO (Last-in First-out) abstract data type where the last element inserted is the first one to be removed. This is a singly-linked list implementation where all operations are done at the head of the list. Push and Pop are equivalent to inserting and removing an element at the head of the list respectively.

```
    ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐
    │ G │ --> │ F │ --> │ E │ --> │ D │ --> │ C │ --> │ B │ --> │ A │ --> NULL
    └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘
      │
     top

    Pop(G)
    ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐
    │ F │ --> │ E │ --> │ D │ --> │ C │ --> │ B │ --> │ A │ --> NULL
    └───┘     └───┘     └───┘     └───┘     └───┘     └───┘
      │
     top

    Push(H)
    ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐
    │ H │ --> │ F │ --> │ E │ --> │ D │ --> │ C │ --> │ B │ --> │ A │ --> NULL
    └───┘     └───┘     └───┘     └───┘     └───┘     └───┘     └───┘
      │
     top
```

### StackArray

A stack array is the implementation of a stack using an internal buffer. It is very space efficient and unlike a queue it does not need to be a circular buffer, making its implementation much simpler and the buffer reallocation. Since Push and Pop are only done at one end of the buffer, no elements will ever need to be shifted.

```
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ A │ B │ C | D | E | F | G | H | I |   |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
                                      │
                                     top
    Push(J)
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ A │ B │ C | D | E | F | G | H | I | J |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
                                          │
                                         top
    Push(K)
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    │ A │ B │ C | D | E | F | G | H | I | J | K |   |   |   |   |   |   |   |   |   |   |
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
                                              │
                                             top
```

### TreeSet

Not implemented yet.

### TreeMap

Not implemented yet.

### Trie

Not implemented yet.

### UnrolledLinkedList

Not implemented yet.

## Ideas

A Wrapper that operates relative to a global variable that simulates an object:

```c
// global scope in source file
static Array GlobalArray = NULL;

// in main function
// This structure comes with all array functions in it
struct ArrayWrapper array_w;

// sets GlobalArray = my_array
arr_wrap_init(array_w, my_array);

// Adds an element 20 at position 10
// Note that there is no need to pass in a my_array reference
// Because add() function operates relative to GlobalArray
array_w->add(20, 10);

// Frees array_w and sets GlobalArray = NULL
arr_desc_free(array_w);
```

[arr]: #array
[asl]: #associativelist
[avl]: #avltree
[bst]: #binarysearchtree
[bnh]: #binomialheap
[bit]: #bitarray
[btr]: #btree
[cll]: #circularlinkedlist
[cql]: #circularqueuelist
[deq]: #deque
[dqa]: #dequearray
[dic]: #dictionary
[dll]: #doublylinkedlist
[dar]: #dynamicarray
[fbh]: #fibonacciheap
[hmp]: #hashmap
[hst]: #hashset
[hep]: #heap
[mhm]: #multihashmap
[mtm]: #multitreemap
[prh]: #priorityheap
[prq]: #priorityqueue
[que]: #queue
[qua]: #queuearray
[rdt]: #radixtree
[rbt]: #redblacktree
[sll]: #singlylinkedlist
[skp]: #skiplist
[sar]: #sortedarray
[sli]: #sortedlist
[shm]: #sortedhashmap
[shs]: #sortedhashset
[spt]: #splaytree
[stk]: #stack
[sta]: #stackarray
[trs]: #treeset
[trm]: #treemap
[tri]: #trie
[ull]: #unrolledlinkedlist