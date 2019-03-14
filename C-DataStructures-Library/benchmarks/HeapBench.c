/**
 * @file HeapBench.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 22/02/2019
 */

#include <inttypes.h>
#include "Heap.h"
#include "Clock.h"
#include "Utility.h"

void
hep_bench_IO(unsigned_t elements, unsigned_t iterations)
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

    Heap_t *heap = hep_new(interface, MaxHeap);

    if (!heap)
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
        hep_free(heap);
        clk_free(stopwatch);
        interface_free(interface);
        return;
    }

    int64_t min = elements * (-1);
    int64_t max = elements;
    void *element = NULL;
    bool success;
    int64_t **buffer = malloc(sizeof(int64_t*) * elements);
    for (unsigned_t i = 0; i < iterations; i++)
    {
        // Insertion
        clk_start(stopwatch);
        for (unsigned_t j = 0; j < elements; j++)
        {
            element = new_int64_t(random_int64_t(min, max));
            if (!hep_insert(heap, element))
            {
                free(element);
                printf("ERROR!0\n");
            }
        }
        clk_stop(stopwatch);
        insertion_timings[insertion_total++] = stopwatch->time;

        clk_reset(stopwatch);

        // Decrease keys
        clk_start(stopwatch);
        for (unsigned_t j = 0; j < elements; j++)
        {
            element = hep_peek(heap);
            *(int64_t *)element -= random_int64_t(20, 200);
            if (!hep_heapify(heap))
                printf("ERROR!1\n");
        }
        clk_stop(stopwatch);
        search_timings[search_total++] = stopwatch->time;

        clk_reset(stopwatch);

        // Removal
        int64_t t = 0;
        clk_start(stopwatch);
        while (!hep_empty(heap))
        {
            success = hep_remove(heap, &element);
            if (!success)
                printf("ERROR!2\n");
            buffer[t++] = (int64_t*)element;
        }
        clk_stop(stopwatch);
        removal_timings[removal_total++] = stopwatch->time;

        if (hep_count(heap) != 0)
            printf("ERROR!3\n");

        bool is_sorted = true;

        for (unsigned_t j = 0; j < elements - 1; j++)
        {
            if (*buffer[j] < *buffer[j + 1])
            {
                is_sorted = false;
                break;
            }
        }

        if (!is_sorted)
            printf("ERROR!4\n");

        for (unsigned_t j = 0; j < elements; j++)
        {
            free(buffer[i]);
            buffer[i] = NULL;
        }

        clk_reset(stopwatch);
        hep_erase(heap);
    }

    free(buffer);

    // The result will be sum / iterations
    double insertion_sum = 0.0, search_sum = 0.0, removal_sum = 0.0;

    for (unsigned_t i = 0; i < iterations; i++)
    {
        insertion_sum += insertion_timings[i];
        search_sum += search_timings[i];
        removal_sum += removal_timings[i];
    }

    hep_free(heap);
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
    printf("  Average decr-key time  : %lf seconds\n", search_sum / (double)iterations);
    printf("+--------------------------------------------------+\n");
}

// Runs all Heap benchmarks
void HeapBench(void)
{
    printf("+------------------------------------------------------------+\n");
    printf("|                       Heap Benchmark                       |\n");
    printf("+------------------------------------------------------------+\n");

    hep_bench_IO(100000, 100);
    hep_bench_IO(1000000, 10);
    hep_bench_IO(10000000, 1);

    printf("\n");
}