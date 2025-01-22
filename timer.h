/*
 * MIT License
 * 
 * Copyright (c) 2025 Felipe Neves
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TIMER_MODULE_H
#define TIMER_MODULE_H

#include <stdint.h>
#include <stdbool.h>
#include "priority_queue.h"

/**
 * struct timer - Timer structure for managing expiration and callbacks
 * @node: Priority queue node for scheduling
 * @expiry: Tick count when the timer expires
 * @period: Period of the timer in ticks (0 for non-periodic)
 * @callback: Function to call when the timer expires
 * @data: User-defined data passed to the callback
 */
struct timer {
    struct heap_node node;
    uint64_t expiry;
    uint64_t period;
    void (*callback)(struct timer *timer, void *data);
    void *data;
};

/**
 * timer_module_init - Initialize the global timer module
 */
void timer_module_init(void);

/**
 * timer_init - Initialize a timer
 * @timer: Pointer to the timer structure
 * @callback: Callback function to invoke when the timer expires
 * @data: User-defined data passed to the callback
 */
void timer_init(struct timer *timer, void (*callback)(struct timer *timer, void *data), void *data);

/**
 * timer_start - Start or restart a timer
 * @timer: Pointer to the timer structure
 * @ticks: Number of ticks before the timer expires
 * @periodic: Whether the timer should repeat periodically
 */
void timer_start(struct timer *timer, uint64_t ticks, bool periodic);

/**
 * timer_stop - Stop a timer
 * @timer: Pointer to the timer structure
 */
void timer_stop(struct timer *timer);

/**
 * timer_set_period - Set a new period or expiry time for a timer
 * @timer: Pointer to the timer structure
 * @ticks: New period or expiry time in ticks
 */
void timer_set_period(struct timer *timer, uint64_t ticks);

/**
 * timer_increment_tick - Increment the global tick counter and handle expired timers
 */
void timer_increment_tick(void);

#endif /* TIMER_MODULE_H */

