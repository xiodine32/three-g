#include "timers.h"

static struct timer {
    timer_cb callback;

    int index;
    double last_tick;
    double target_ticks_per_second;

    timer *next;
} * first = NULL, * last = NULL;

static void timer_d();

void timer_new(int timer_index,
    double target_ticks_per_second,
    timer_cb callback) {
    timer *new_timer = (timer *)malloc(sizeof(timer));

    new_timer->callback = callback;
    new_timer->index = timer_index;
    new_timer->target_ticks_per_second = 1 / target_ticks_per_second;
    new_timer->last_tick = glfwGetTime();
    new_timer->next = NULL;


    if (last != NULL) {
        last->next = new_timer;
    }

    last = new_timer;

    if (first == NULL) {
        first = last;
    }

    d("[timer_new]: %p - index: %d FPS: %5.2lf\n",
        new_timer,
        timer_index,
        target_ticks_per_second);

    timer_d();
}

static void timer_d() {
    #ifndef DEBUG
    return;
    #endif
    timer *pointer = first;

    d("[timer_d]: first: %p last: %p list: ", first, last);

    while (pointer != NULL) {
        d("%p %d -> ", pointer, pointer->index);

        pointer = pointer->next;
    }
    d("END\n");
}

void timer_delete(int timer_index) {
    timer *pointer = first;

    // empty list

    if (pointer == NULL) {
        d("[timer_delete]: Cannot delete from an empty list!\n");
        timer_d();
        return;
    }

    // first element

    if (pointer->index == timer_index) {
        d("[timer_delete]: removing first element\n");
        timer *to_delete = pointer;

        // dereference
        if (last == first) {
            d("[timer_delete]: also removing last element\n");
            last = NULL;
        }
        first = first->next;

        // delete
        free(to_delete);

        timer_d();
        return;
    }

    // middle element

    while (pointer->next != NULL && pointer->next->index != timer_index) {
        pointer = pointer->next;
    }


    if (pointer->next == NULL) {
        d("[timer_delete]: index was not found!\n");
        timer_d();
        return;
    }

    timer *to_delete = pointer->next;


    // last element

    if (last == pointer->next) {
        d("[timer_delete]: removing last element\n");
        last = pointer;
    } else {
        d("[timer_delete]: removing middle element\n");
    }

    // dereference

    pointer->next = pointer->next->next;

    // delete
    free(to_delete);
    timer_d();
}

void timer_run() {
    timer *pointer = first;

    while (pointer != NULL) {
        while (glfwGetTime() - pointer->last_tick >
            pointer->target_ticks_per_second) {
            pointer->last_tick += pointer->target_ticks_per_second;
            pointer->callback();
        }

        pointer = pointer->next;
    }
}
