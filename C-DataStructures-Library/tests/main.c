#include "Tests.h"

int main()
{
    srand((unsigned)time(NULL));

    BitArrayTests();
    DequeArrayTests();
    DequeTests();
    DoublyLinkedListTests();
    DynamicArrayTests();
    CircularLinkedListTests();
    QueueArrayTests();
    QueueTests();
    RedBlackTreeTests();
    SinglyLinkedListTests();
    SortedListTests();
    StackArrayTests();
    StackTests();

    FinalReport();

    return 0;
}