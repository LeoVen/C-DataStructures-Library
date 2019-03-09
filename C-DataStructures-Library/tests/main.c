#include "Tests.h"

int main(void)
{
    srand((unsigned)time(NULL));

    printf("+--------------------------------------------------+\n");
    printf("|                       Tests                      |\n");
    printf("+--------------------------------------------------+\n\n");

    ArrayTests();
    AssociativeListTests();
    AVLTreeTests();
    BinarySearchTreeTests();
    BitArrayTests();
    CircularLinkedListTests();
    DequeArrayTests();
    DequeListTests();
    DoublyLinkedListTests();
    DynamicArrayTests();
    HeapTests();
    PriorityListTests();
    QueueArrayTests();
    QueueListTests();
    RedBlackTreeTests();
    SinglyLinkedListTests();
    SortedListTests();
    StackArrayTests();
    StackListTests();

    FinalReport();
}