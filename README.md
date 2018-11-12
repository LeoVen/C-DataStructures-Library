# C-DataStructures-Library

## Planned Data Structures

|   Structure Name   |   Source Code  |    Iterator    |      Tests     |  Documentation |
| :----------------- | :------------: | :------------: | :------------: | :------------: |
|       Array        | `[##########]` | `[__________]` | `[__________]` | `[#_________]` |
|      AVLTree       | `[##########]` | `[__________]` | `[__________]` | `[#_________]` |
|  BinarySearchTree  | `[##########]` | `[__________]` | `[__________]` | `[##________]` |
|    BinomialHeap    | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|       BTree        | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|   CircularBuffer   | `[##########]` | `[__________]` | `[__________]` | `[#_________]` |
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
|    RedBlackTree    | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|  SinglyLinkedList  | `[#########_]` | `[__________]` | `[__________]` | `[######____]` |
|      SkipList      | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|    SortedArray     | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|     SortedList     | `[##########]` | `[##########]` | `[__________]` | `[##########]` |
|  SortedUnionList   | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|   SortedHashMap    | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|    SparseMatrix    | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|     SplayTree      | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|       Stack        | `[##########]` | `[__________]` | `[__________]` | `[#######___]` |
|     StackArray     | `[##########]` | `[__________]` | `[__________]` | `[######____]` |
|      TreeSet       | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|      TreeMap       | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|        Trie        | `[__________]` | `[__________]` | `[__________]` | `[__________]` |
|   __Completed__    |     __14__     |     __1__      |     __0__      |     __1__      |

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

int len = arr_length(my_array);

for (int i = 0; i < len; i++)
{
    // Do something
    // Setting the position i with value (i * i)
    arr_set(my_array, i, i * i);
}
```

Note that the length variable is protected (implementation detail) and only accessible via a function. This is done so to prevent the user from changing the length variable of the array's struct, possibly causing run-time errors. Also access to the buffer is protected and you must use ```arr_set()``` to set a value in the array and ```arr_get()``` to get the value from the array.

### AVLTree

An AVL tree is a balanced binary-search tree.
