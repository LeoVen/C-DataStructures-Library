#include "Util.h"
#include "Tests.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "Deque.h"
#include "PriorityQueue.h"
#include "DynamicArray.h"
#include "StackArray.h"
#include "QueueArray.h"
#include "Array.h"
#include "CircularLinkedList.h"
#include "DequeArray.h"
#include "BinarySearchTree.h"
#include "AVLTree.h"
#include "SortedList.h"
#include "BitArray.h"

int main()
{
    srand(time(NULL));

    BitArrayTests();
    DequeArrayTests();
    DequeTests();
    DoublyLinkedListTests();
    DynamicArrayTests();
    CircularLinkedListTests();
    QueueArrayTests();
    QueueTests();
    SinglyLinkedListTests();
    SortedListTests();
    StackArrayTests();
    StackTests();

    return 0;
}