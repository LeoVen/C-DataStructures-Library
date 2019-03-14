/**
 * @file RedBlackTreeBench.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 20/12/2018
 */

#include <inttypes.h>
#include "RedBlackTree.h"
#include "Clock.h"
#include "Utility.h"

void
rbt_bench_IO(unsigned_t elements, unsigned_t iterations)
{
    srand(5113);

    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, NULL, NULL);

    if (!interface)
        return;

    Clock_t *stopwatch = clk_new(iterations);

    if (!stopwatch)
    {
        interface_free(interface);
        return;
    }

    RedBlackTree_t *tree = rbt_new(interface);

    if (!tree)
    {
        interface_free(interface);
        clk_free(stopwatch);
        return;
    }

    double *insertion_timings = malloc(sizeof(double) * iterations);
    double *removal_timings = malloc(sizeof(double) * iterations);
    double *search_timings = malloc(sizeof(double) * iterations);

    unsigned_t insertion_total = 0, removal_total = 0, search_total = 0;

    if (!insertion_timings || !removal_timings || !search_timings)
    {
        rbt_free(tree);
        clk_free(stopwatch);
        interface_free(interface);
        return;
    }

    int64_t min = elements * (-1);
    int64_t max = elements;
    void *element = NULL;
    bool success;
    for (unsigned_t i = 0; i < iterations; i++)
    {
        // Insertion
        clk_start(stopwatch);
        for (unsigned_t j = 0; j < elements; j++)
        {
            element = new_int64_t(random_int64_t(min, max));
            success = rbt_insert(tree, element);
            if (!success)
                free(element);
        }
        clk_stop(stopwatch);
        insertion_timings[insertion_total++] = stopwatch->time;

        clk_reset(stopwatch);

        // Search
        clk_start(stopwatch);
        for (int64_t j = min; j <= max; j++)
        {
            success = rbt_contains(tree, &j);
        }
        clk_stop(stopwatch);
        search_timings[search_total++] = stopwatch->time;

        clk_reset(stopwatch);

        // Removal
        int64_t *search = new_int64_t(min);
        clk_start(stopwatch);
        for (int64_t j = min; j <= max; j++)
        {
            *search = j;
            success = rbt_remove(tree, search);
        }
        clk_stop(stopwatch);
        removal_timings[removal_total++] = stopwatch->time;
        free(search);

        if (rbt_size(tree) != 0)
            printf("ERROR\n");

        clk_reset(stopwatch);
        rbt_erase(tree);
    }

    // The result will be sum / iterations
    double insertion_sum = 0.0, search_sum = 0.0, removal_sum = 0.0;

    for (unsigned_t i = 0; i < iterations; i++)
    {
        insertion_sum += insertion_timings[i];
        search_sum += search_timings[i];
        removal_sum += removal_timings[i];
    }

    rbt_free(tree);
    clk_free(stopwatch);
    interface_free(interface);
    free(insertion_timings);
    free(search_timings);
    free(removal_timings);

    printf("+--------------------------------------------------+\n");
    printf("  Total elements added   : %" PRIuMAX "\n", elements);
    printf("  Total iterations       : %" PRIuMAX "\n", iterations);
    printf("+--------------------------------------------------+\n");
    printf("  Average insertion time : %lf seconds\n", insertion_sum / (double)iterations);
    printf("  Average removal time   : %lf seconds\n", removal_sum / (double)iterations);
    printf("  Average search time    : %lf seconds\n", search_sum / (double)iterations);
    printf("+--------------------------------------------------+\n");
}

// Runs all RedBlackTree benchmarks
void RedBlackTreeBench(void)
{
    printf("+------------------------------------------------------------+\n");
    printf("|                   RedBlackTree Benchmark                   |\n");
    printf("+------------------------------------------------------------+\n");

    rbt_bench_IO(100000, 100);
    rbt_bench_IO(1000000, 10);
    rbt_bench_IO(10000000, 1);

    printf("\n");
}