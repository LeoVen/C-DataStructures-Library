/**
 * @file Clock.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 20/12/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_CLOCK_H
#define C_DATASTRUCTURES_LIBRARY_CLOCK_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \brief A stopwatch wrapper.
struct Clock_s
{
    /// \brief If the clock is running or not.
    ///
    /// True if the clock is running, false if the clock is stopped.
    bool running;

    /// \brief Last calculated time.
    ///
    /// The last calculated time.
    double time;

    /// \brief Laps buffer.
    ///
    /// Stores the laps time
    double *buffer;

    /// \brief Buffer size.
    ///
    /// The fixed size of the buffer.
    unsigned_t size;

    /// \brief How many laps are stored in the buffer.
    ///
    /// How many laps are stored in the buffer.
    size_t count;

    /// \brief Current running time.
    ///
    /// Current running time.
    clock_t timer;

    /// \brief Current lap running time.
    ///
    /// Current lap running time.
    clock_t lap_timer;
};

typedef struct Clock_s Clock_t;

typedef struct Clock_s *Clock;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Clock_t *
clk_new(unsigned_t storage_size);

void
clk_free(Clock_t *clk);

////////////////////////////////////////////////////////// CLOCK OPERATIONS ///

bool
clk_start(Clock_t *clk);

bool
clk_stop(Clock_t *clk);

bool
clk_lap(Clock_t *clk);

bool
clk_reset(Clock_t *clk);

/////////////////////////////////////////////////////////////// CLOCK STATE ///

bool
clk_running(Clock_t *clk);

bool
clk_stopped(Clock_t *clk);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_CLOCK_H
