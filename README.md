# Priority Queue Implementation

This project provides an intrusive priority queue implementation using a heap-based structure. It is designed for efficient priority-based node management and is implemented in C.

## Features
- **Intrusive Design**: Each node contains its own pointers, reducing the need for external memory management.
- **Custom Comparison**: Users can define their own comparison function for flexible priority handling.
- **Heap Operations**: Includes insert and pop operations optimized for performance.
- **Inline Optimization**: Critical internal functions are inlined for reduced overhead.
- **Error Handling**: Functions return negative error codes (`-EINVAL`, `-ENOENT`) in case of invalid inputs or operations.

## Files
- `priority_queue.h`: Header file defining the public interface of the priority queue.
- `priority_queue.c`: Implementation file with the logic for heap-based operations.

## License
This project is licensed under the MIT License. See the license details in the header of the source and header files.

## How to Use
1. Include the `priority_queue.h` file in your project.
2. Define a comparison function for your heap nodes.
3. Initialize a `priority_queue` structure using `pq_init`.
4. Insert nodes with `pq_insert`.
5. Remove nodes with `pq_pop`.

### Example
```c
#include "priority_queue.h"
#include <stdio.h>

int compare_nodes(struct heap_node *a, struct heap_node *b)
{
    return (int)(intptr_t)a->parent - (int)(intptr_t)b->parent;
}

int main()
{
    struct priority_queue pq;
    pq_init(&pq, compare_nodes);

    struct heap_node nodes[3];
    for (int i = 0; i < 3; ++i) {
        nodes[i].parent = (struct heap_node *)(intptr_t)(3 - i); // Assign priorities
        pq_insert(&pq, &nodes[i]);
    }

    for (int i = 0; i < 3; ++i) {
        struct heap_node *node = pq_pop(&pq);
        if (node) {
            printf("Popped node with priority: %d\\n", (int)(intptr_t)node->parent);
        }
    }

    return 0;
}

