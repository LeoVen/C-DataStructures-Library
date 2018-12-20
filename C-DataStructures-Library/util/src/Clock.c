/**
 * @file Clock.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 20/12/2018
 */

#include "Clock.h"

Clock_t *
clk_new(unsigned_t storage_size)
{
    Clock_t *clk = malloc(sizeof(Clock_t));

    if (!clk)
        return NULL;

    clk->buffer = malloc(sizeof(double) * storage_size);

    if (!clk->buffer)
    {
        free(clk);
        return NULL;
    }

    clk->running = false;
    clk->time = 0.0;
    clk->size = storage_size;
    clk->count = 0;
    clk->timer = 0;
    clk->lap_timer = 0;

    return clk;
}

void
clk_free(Clock_t *clk)
{
    free(clk->buffer);

    free(clk);
}

bool
clk_start(Clock_t *clk)
{
    if (clk_running(clk))
        return false;
    
    clk->timer = clock();
    clk->lap_timer = clock();

    clk->running = true;

    return true;
}

bool
clk_stop(Clock_t *clk)
{
    if (clk_stopped(clk))
        return false;

    clock_t T = clock();

    clk->time += (double)(T - clk->timer) / (double)CLOCKS_PER_SEC;

    clk->running = false;

    return true;
}

bool
clk_lap(Clock_t *clk)
{
    if (clk_stopped(clk))
        return false;

    // FULL
    if (clk->count == clk->size)
        return false;

    clock_t T = clock();

    clk->buffer[clk->count] = (double)(T - clk->lap_timer) / (double)CLOCKS_PER_SEC;

    clk->count++;

    clk->lap_timer = T;

    return true;
}

bool
clk_reset(Clock_t *clk)
{
    if (clk_running(clk))
        return false;

    clk->time = 0.0;
    clk->count = 0;

    return true;
}

bool
clk_running(Clock_t *clk)
{
    return clk->running;
}

bool
clk_stopped(Clock_t *clk)
{
    return !clk->running;
}
