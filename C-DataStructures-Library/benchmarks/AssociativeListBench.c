/**
 * @file AssociativeListBench.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 08/02/2018
 */

#include <inttypes.h>
#include "AssociativeList.h"
#include "Clock.h"
#include "Utility.h"

void
ali_bench_IO(unsigned_t elements, unsigned_t iterations, bool duplicate_keys)
{
    srand(5113);

    Interface_t *key_interface = interface_new(compare_string, copy_string,
            display_string, free, hash_string, NULL);
    Interface_t *value_interface = interface_new(compare_int32_t, copy_int32_t,
            display_int32_t, free, hash_int32_t, NULL);

    char **keys = malloc(sizeof(char*) * (size_t)elements);

    if (!key_interface || ! value_interface || !keys)
        return;

    Clock_t *stopwatch = clk_new(iterations);

    if (!stopwatch)
        return;

    AssociativeList_t *list = ali_new(key_interface, value_interface, duplicate_keys);

    if (!list)
        return;

    double *insertion_timings = malloc(sizeof(double) * (size_t)iterations);
    double *removal_timings = malloc(sizeof(double) * (size_t)iterations);
    double *search_timings = malloc(sizeof(double) * (size_t)iterations);

    unsigned_t insertion_total = 0, removal_total = 0, search_total = 0;

    if (!insertion_timings || !removal_timings || !search_timings)
    {
        ali_free(list);
        clk_free(stopwatch);
        interface_free(key_interface);
        interface_free(value_interface);
        return;
    }

    const integer_t V = 9999999999;
    void *value = NULL, *result = NULL;
    bool success;
    for (unsigned_t i = 0; i < iterations; i++)
    {
        // Fill keys array
        for (unsigned_t k = 0; k < elements; k++)
        {
            keys[k] = random_string(5, 1000, false);
        }

        clk_reset(stopwatch);

        // Insertion
        clk_start(stopwatch);
        for (unsigned_t j = 0; j < elements; j++)
        {
            value = new_int64_t(random_int64_t(V * -1, V));

            success = ali_insert(list, keys[j], value);

            if (!success)
                free(value);
        }
        clk_stop(stopwatch);
        insertion_timings[insertion_total++] = stopwatch->time;

        clk_reset(stopwatch);

        // Search
        clk_start(stopwatch);
        for (int64_t j = 0; j < elements; j++)
        {
            result = ali_get(list, keys[j]);
        }
        clk_stop(stopwatch);
        search_timings[search_total++] = stopwatch->time;

        clk_reset(stopwatch);

        // Removal
        clk_start(stopwatch);
        for (int64_t j = 0; j < elements; j++)
        {
            success = ali_remove(list, keys[j], &result);

            free(result);

            if (!success)
                free(keys[j]);
        }
        clk_stop(stopwatch);
        removal_timings[removal_total++] = stopwatch->time;

        if (ali_length(list) != 0)
            printf("ERROR\n");

        ali_erase(list);
    }

    // The result will be sum / iterations
    double insertion_sum = 0.0, search_sum = 0.0, removal_sum = 0.0;

    for (unsigned_t i = 0; i < iterations; i++)
    {
        insertion_sum += insertion_timings[i];
        search_sum += search_timings[i];
        removal_sum += removal_timings[i];
    }

    ali_free(list);
    clk_free(stopwatch);
    interface_free(key_interface);
    interface_free(value_interface);
    free(insertion_timings);
    free(search_timings);
    free(removal_timings);
    free(keys);

    printf("+--------------------------------------------------+\n");
    printf("  Total elements added   : %" PRIuMAX "\n", elements);
    printf("  Total iterations       : %" PRIuMAX "\n", iterations);
    printf("  Duplicate Keys         : %s\n", duplicate_keys ? "YES" : "NO");
    printf("+--------------------------------------------------+\n");
    printf("  Average insertion time : %lf seconds\n", insertion_sum / (double)iterations);
    printf("  Average removal time   : %lf seconds\n", removal_sum / (double)iterations);
    printf("  Average search time    : %lf seconds\n", search_sum / (double)iterations);
    printf("+--------------------------------------------------+\n");
}

// Runs all AssociativeList benchmarks
void AssociativeListBench(void)
{
    printf("+------------------------------------------------------------+\n");
    printf("|                 AssociativeList Benchmark                  |\n");
    printf("+------------------------------------------------------------+\n");

    ali_bench_IO( 10000, 10, true);
    ali_bench_IO( 10000, 10, false);
    ali_bench_IO( 50000,  2, true);
    ali_bench_IO( 50000,  2, false);
    ali_bench_IO(100000,  1, true);
    ali_bench_IO(100000,  1, false);

    printf("\n");
}
