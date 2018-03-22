#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

#include "all.h"

#define DEFAULT_TIMER 0

typedef void (*timer_cb)();

// creates a new timer, timer_index should be > 0.
void timer_new(int timer_index,
    double target_ticks_per_second,
    timer_cb callback);

// deletes first timer with it's respective timer_index.
void timer_delete(int timer_index);

// runs through timers.
void timer_run();

#endif  // SRC_TIMERS_H_
