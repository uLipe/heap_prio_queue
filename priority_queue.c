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

#include "priority_queue.h"
#include <errno.h>

#ifdef __GNUC__
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

static inline void swap_nodes(struct heap_node *a, struct heap_node *b)
{
    struct heap_node temp = *a;
    *a = *b;
    *b = temp;

    if (a->parent == b) {
        a->parent = b->parent;
        b->parent = a;
    } else if (b->parent == a) {
        b->parent = a->parent;
        a->parent = b;
    } else {
        struct heap_node *temp_parent = a->parent;
        a->parent = b->parent;
        b->parent = temp_parent;
    }
}

int pq_init(struct priority_queue *pq, int (*compare)(struct heap_node *, struct heap_node *))
{
    if (unlikely(!pq || !compare)) {
        return -EINVAL;
    }

    pq->head = NULL;
    pq->tail = NULL;
    pq->root = NULL;
    pq->compare = compare;

    return 0;
}

int pq_insert(struct priority_queue *pq, struct heap_node *node)
{
    if (unlikely(!pq || !node)) {
        return -EINVAL;
    }

    if (!pq->head) {
        pq->head = pq->tail = pq->root = node;
        node->parent = node->next = node->prev = NULL;
    } else {
        pq->tail->next = node;
        node->prev = pq->tail;
        pq->tail = node;
        node->next = NULL;
    }

    return 0;
}

struct heap_node *pq_pop(struct priority_queue *pq)
{
    if (unlikely(!pq)) {
        return NULL;
    }

    if (!pq->head) {
        return NULL;
    }

    struct heap_node *root = pq->root;

    if (pq->head == pq->tail) {
        pq->head = pq->tail = pq->root = NULL;
        return root;
    }

    pq->root = pq->tail;

    if (pq->tail->prev) {
        pq->tail->prev->next = NULL;
    }

    pq->tail = pq->tail->prev;
    pq->root->next = pq->root->prev = NULL;

    return root;
}

struct heap_node *pq_peek(struct priority_queue *pq)
{
    if (unlikely(!pq || !pq->root)) {
        return NULL;
    }

    return pq->root;
}

void pq_reorder(struct priority_queue *pq)
{
    if (unlikely(!pq || !pq->head)) {
        return;
    }

    struct heap_node *current = pq->root;
    while (1) {
        struct heap_node *largest = current;

        if (current->next && pq->compare(current->next, largest) > 0) {
            largest = current->next;
        }

        if (current->prev && pq->compare(current->prev, largest) > 0) {
            largest = current->prev;
        }

        if (largest == current) {
            break;
        }

        swap_nodes(current, largest);
    }
}

