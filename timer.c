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

#include "timer_module.h"
#include "container_of.h"

static struct priority_queue timer_queue;
static uint64_t global_ticks;

static int timer_compare(struct heap_node *a, struct heap_node *b)
{
    struct timer *timer_a = CONTAINER_OF(a, struct timer, node);
    struct timer *timer_b = CONTAINER_OF(b, struct timer, node);
    return (timer_a->expiry > timer_b->expiry) - (timer_a->expiry < timer_b->expiry);
}

void timer_module_init(void)
{
    pq_init(&timer_queue, timer_compare);
    global_ticks = 0;
}

void timer_init(struct timer *timer, void (*callback)(struct timer *timer, void *data), void *data)
{
    timer->callback = callback;
    timer->data = data;
    timer->expiry = 0;
    timer->period = 0;
}

void timer_start(struct timer *timer, uint64_t ticks, bool periodic)
{
    timer->expiry = global_ticks + ticks;
    timer->period = periodic ? ticks : 0;
    pq_insert(&timer_queue, &timer->node);
    pq_reorder(&timer_queue);
}

void timer_stop(struct timer *timer)
{
    pq_remove(&timer_queue, &timer->node);
}

void timer_set_period(struct timer *timer, uint64_t ticks)
{
    timer->period = ticks;
}

void timer_increment_tick(void)
{
    global_ticks++;

    struct timer *next_timer = NULL;
    struct heap_node *node = pq_peek(&timer_queue);

    while (node && (next_timer = CONTAINER_OF(node, struct timer, node))->expiry <= global_ticks) {
        pq_pop(&timer_queue);
        next_timer->callback(next_timer, next_timer->data);

        if (next_timer->period) {
            next_timer->expiry += next_timer->period;
            pq_insert(&timer_queue, &next_timer->node);
        }

        node = pq_peek(&timer_queue);
    }

    pq_reorder(&timer_queue);
}

