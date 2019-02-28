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
    DequeTests();
    DoublyLinkedListTests();
    DynamicArrayTests();
    HeapTests();
    PriorityListTests();
    QueueArrayTests();
    QueueTests();
    RedBlackTreeTests();
    SinglyLinkedListTests();
    SortedListTests();
    StackArrayTests();
    StackTests();

    FinalReport();
}