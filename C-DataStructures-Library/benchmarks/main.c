#include "Benchmarks.h"

int main(void)
{
    printf("+--------------------------------------------------+\n");
    printf("|                    Benchmarks                    |\n");
    printf("+--------------------------------------------------+\n\n");

    AssociativeListBench();
    AVLTreeBench();
    HeapBench();
    RedBlackTreeBench();
}
