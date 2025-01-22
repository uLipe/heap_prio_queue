# Data Structures and Utilities in C

This project contains several commonly used data structures and utilities implemented in C. Each implementation is optimized for performance, follows the Linux kernel coding style, and is designed for flexibility and reusability.

## Included Modules

1. [Priority Queue](#priority-queue)
2. [Intrusive Doubly Linked List](#intrusive-doubly-linked-list)
3. [Container Of Macro](#container-of-macro)
4. [Ring Buffer](#ring-buffer)
5. [Ping-Pong Buffer (Lock-Free Buffer)](#ping-pong-buffer-lock-free-buffer)

---

## Priority Queue

### Overview
An intrusive priority queue implementation using a heap-based structure. Designed for efficient priority-based node management.

### Features
- **Intrusive Design**: Each node contains its own pointers, reducing external memory management.
- **Custom Comparison**: Flexible priority handling using user-defined comparison functions.
- **Heap Operations**: Optimized insert and pop operations.
- **Inline Optimization**: Reduced overhead with inlined critical functions.
- **Error Handling**: Returns negative error codes (`-EINVAL`, `-ENOENT`).

### Files
- `priority_queue.h`: Header file defining the public interface.
- `priority_queue.c`: Source file implementing the logic.

### Usage Example
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
        nodes[i].parent = (struct heap_node *)(intptr_t)(3 - i);
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
```

---

## Intrusive Doubly Linked List

### Overview
A doubly linked list implementation that minimizes overhead by embedding pointers directly into the nodes.

### Features
- **Intrusive Design**: The list nodes are embedded in the structures being managed.
- **O(1) Complexity**: Insert, remove, and traversal operations.
- **Traversal Support**: Safe iteration using a `list_for_each_safe` macro.

### File
- `intrusive_list.h`: Header-only implementation.

### Usage Example
```c
#include "intrusive_list.h"
#include <stdio.h>

int main(void)
{
    struct list_head my_list;
    struct list_node nodes[3];

    list_init(&my_list);

    for (int i = 0; i < 3; ++i) {
        list_append(&my_list, &nodes[i]);
    }

    struct list_node *node, *tmp;
    list_for_each_safe(&my_list, node, tmp) {
        printf("Node: %p\\n", (void *)node);
        list_remove(node);
    }

    printf("List is empty: %s\\n", list_is_empty(&my_list) ? "true" : "false");

    return 0;
}
```

---

## Container Of Macro

### Overview
A utility macro to calculate the address of a container structure from a member pointer.

### Features
- **Generic**: Works with any structure and member.
- **Lightweight**: Resolves the container structure address at compile time.

### File
- `container_of.h`: Header-only implementation.

### Usage Example
```c
#include "container_of.h"
#include <stdio.h>

struct container {
    int value;
    struct list_node list;
};

int main(void)
{
    struct container my_container;
    struct list_node *node = &my_container.list;

    struct container *retrieved = CONTAINER_OF(node, struct container, list);
    printf("Retrieved container address: %p\\n", (void *)retrieved);

    return 0;
}
```

---

## Ring Buffer

### Overview
A fast, single-producer, single-consumer ring buffer implementation using bitmask-based index calculations.

### Features
- **Fast Indexing**: Uses power-of-two sizes for efficient bitmasking.
- **Dynamic Capacity**: Automatically adjusts size to the nearest power of two.
- **Error Handling**: Returns error codes for full or empty conditions.
- **Stream Support**: Copy multiple bytes into the buffer with safety checks.

### File
- `ring_buffer.h`: Header-only implementation.

### Usage Example
```c
#include "ring_buffer.h"
#include <stdio.h>

int main(void)
{
    uint8_t buffer[16];
    struct ring_buffer rb;

    ring_buffer_init(&rb, buffer, sizeof(buffer));

    uint8_t data_to_push[] = {1, 2, 3};
    if (ring_buffer_copy_from_stream(&rb, data_to_push, sizeof(data_to_push)) >= 0) {
        printf("Data pushed successfully.\\n");
    }

    uint8_t byte;
    while (ring_buffer_pop(&rb, &byte) == 0) {
        printf("Popped: %u\\n", byte);
    }

    return 0;
}
```

---

## Ping-Pong Buffer (Lock-Free Buffer)

### Overview
A lock-free, single-producer, single-consumer buffer implemented using double buffering.

### Features
- **Lock-Free**: Uses atomic index swapping for safe access.
- **Double Buffering**: Two buffers alternately used for read/write operations.
- **Error Handling**: Returns error codes for invalid operations or buffer conditions.

### File
- `ping_pong_buffer.h`: Header-only implementation.

### Usage Example
```c
#include "ping_pong_buffer.h"
#include <stdio.h>

int main(void)
{
    uint8_t buffer1[64];
    uint8_t buffer2[64];
    struct ping_pong_buffer ppb;

    ping_pong_buffer_init(&ppb, buffer1, buffer2, sizeof(buffer1));

    uint8_t data_to_write[] = {42};
    if (ping_pong_buffer_write(&ppb, data_to_write, sizeof(data_to_write)) == 0) {
        printf("Write successful.\\n");
    }

    uint8_t data_read[64];
    if (ping_pong_buffer_read(&ppb, data_read, sizeof(data_to_write)) == 0) {
        printf("Read successful: %u\\n", data_read[0]);
    }

    return 0;
}
```

---

### License
This project is licensed under the MIT License. See the license details in each file.
