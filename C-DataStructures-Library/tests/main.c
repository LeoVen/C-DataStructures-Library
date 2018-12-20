#include "Tests.h"

int main(void)
{
    srand((unsigned)time(NULL));

    printf("+--------------------------------------------------+\n");
    printf("|                       Tests                      |\n");
    printf("+--------------------------------------------------+\n\n");

    AVLTreeTests();
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
}