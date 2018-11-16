# C-DataStructures-Library

## Planned Data Structures

|   Structure Name   |   Source Code  |    Iterator    |      Tests     |  Documentation |
| :----------------- | :------------: | :------------: | :------------: | :------------: |
|       Array        | `[##########]` | `[##########]` | `[__________]` | `[#_________]` |
|      AVLTree       | `[##########]` | `[__________]` | `[__________]` | `[#_________]` |
|  BinarySearchTree  | `[##########]` | `[__________]` | `[__________]` | `[##________]` |
|    BinomialHeap    | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|       BTree        | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
| CircularLinkedList | `[##########]` | `[__________]` | `[__________]` | `[#####_____]` |
|       Deque        | `[##########]` | `[__________]` | `[__________]` | `[######____]` |
|     DequeArray     | `[##########]` | `[__________]` | `[__________]` | `[###_______]` |
|     Dictionary     | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|  DoublyLinkedList  | `[########__]` | `[__________]` | `[__________]` | `[#####_____]` |
|    DynamicArray    | `[##########]` | `[__________]` | `[__________]` | `[##________]` |
|      HashMap       | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|      HashSet       | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|        Heap        | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|    PriorityHeap    | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|   PriorityQueue    | `[##########]` | `[__________]` | `[__________]` | `[#_________]` |
|       Queue        | `[##########]` | `[__________]` | `[__________]` | `[#######___]` |
|     QueueArray     | `[##########]` | `[__________]` | `[__________]` | `[######____]` |
|     RadixTree      | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|    RedBlackTree    | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|  SinglyLinkedList  | `[#########_]` | `[__________]` | `[__________]` | `[######____]` |
|      SkipList      | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|    SortedArray     | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|     SortedList     | `[##########]` | `[##########]` | `[__________]` | `[##########]` |
|   SortedHashMap    | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|     SplayTree      | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|       Stack        | `[##########]` | `[__________]` | `[__________]` | `[#######___]` |
|     StackArray     | `[##########]` | `[__________]` | `[__________]` | `[######____]` |
|      TreeSet       | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|      TreeMap       | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|        Trie        | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|   __Completed__    |     __14__     |     __2__      |     __0__      |     __1__      |

## Generic Data Structures

|   Structure Name   |   Source Code  |    Iterator    |      Tests     |  Documentation |
| :----------------- | :------------: | :------------: | :------------: | :------------: |
|       GArray       | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|      GAVLTree      | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|       GDeque       | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|     GDictionary    | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|      GHashMap      | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|      GHashSet      | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|       GHeap        | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|    GLinkedList     | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|   GPriorityQueue   | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|       GQueue       | `[########__]` | `[__________]` | `[__________]` | `[####______]` |
|     GSkipList      | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|       GStack       | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|      GTreeMap      | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|      GTreeSet      | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|   __Completed__    |     __0__      |     __0__      |     __0__      |     __0__      |

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

### AVLTree

An AVL tree is a type of a tree that has all the properties of a binary search tree with an aditional one: at any given node, the absolute difference between heights of left sub-tree and right sub-tree cannot be greater than 1. This property is essential to keep the tree balanced.

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
* A binary tree with __N__ nodes, the minumum number of levels is <b>Log<sub>2</sub>(N + 1)</b>;
* A binary tree with __N__ leaves nas at least <b>Log<sub>2</sub>(L) + 1</b>.

The problem with binary search trees is that they can get skewed. To solve this there are other variations like AVL trees and Red-black trees that can balance the distribution of nodes. If we add the numbers from 1 to 7 in order this is what would happen to a binary search tree versus an AVL tree:

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

### BTree

Not implemented yet.

### CircularLinkedList

A circular linked list is a singly-linked list where its nodes wrap around making a circular structure.

### Deque

A deque is a double-ended queue implemented as a doubly-linked list. Both operations `enqueue` and `dequeue` can be done to both ends of the queue.

### DequeArray

A deque array is the implementation of a deque using a circular buffer.

### Dictionary

Not implemented yet.

### DoublyLinkedList

### Dynamic Array

### HashMap

Not implemented yet.

### HashSet

Not implemented yet.

### Heap

Not implemented yet.

### PriorityHeap

Not implemented yet.

### PriorityQueue

### Queue

### QueueArray

### RadixTree

Not implemented yet.

### RedBlackTree

Not implemented yet.

### SinglyLinkedList

### SkipList

Not implemented yet.

### SortedArray

Not implemented yet.

### SortedList

### SortedHashMap

Not implemented yet.

### SplayTree

Not implemented yet.

### Stack

### StackArray

### TreeSet

Not implemented yet.

### TreeMap

Not implemented yet.

### Trie

Not implemented yet.