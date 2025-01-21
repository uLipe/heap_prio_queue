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

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

/**
 * struct heap_node - intrusive heap node structure
 * @next: pointer to the next node
 * @prev: pointer to the previous node
 * @parent: pointer to the parent node
 */
struct heap_node {
    struct heap_node *next;
    struct heap_node *prev;
    struct heap_node *parent;
};

/**
 * struct priority_queue - priority queue structure
 * @head: pointer to the head of the queue
 * @tail: pointer to the tail of the queue
 * @root: pointer to the root of the heap
 * @compare: function pointer to the comparison function
 */
struct priority_queue {
    struct heap_node *head;
    struct heap_node *tail;
    struct heap_node *root;
    int (*compare)(struct heap_node *, struct heap_node *);
};

/**
 * pq_init - Initializes a priority queue
 * @pq: pointer to the priority queue structure
 * @compare: function pointer to the comparison function
 * 
 * Return: 0 on success, or -EINVAL if parameters are invalid
 */
int pq_init(struct priority_queue *pq, int (*compare)(struct heap_node *, struct heap_node *));

/**
 * pq_insert - Inserts a node into the priority queue
 * @pq: pointer to the priority queue structure
 * @node: pointer to the heap node to insert
 * 
 * Return: 0 on success, or -EINVAL if parameters are invalid
 */
int pq_insert(struct priority_queue *pq, struct heap_node *node);

/**
 * pq_pop - Pops the highest-priority node from the priority queue
 * @pq: pointer to the priority queue structure
 * 
 * Return: pointer to the highest-priority heap node, or NULL on error
 */
struct heap_node *pq_pop(struct priority_queue *pq);

#endif /* PRIORITY_QUEUE_H */
