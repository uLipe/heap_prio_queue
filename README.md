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

## Fixed-Size Message Queue

### Overview
A fixed-size message queue designed for efficient single-producer, single-consumer communication. It supports operations for pushing, peeking, popping, and flushing messages. The queue ensures that the number of slots is rounded down to the nearest power of two for efficient bitmask-based indexing.

### Features
- **Power-of-Two Slots**: The number of slots is rounded down to the nearest power of two for efficient index calculations.
- **Compile-Time Buffer Declaration**: Use a macro to declare a buffer of the appropriate size for a given number of slots and message size.
- **Efficient Operations**:
    - Push: Add a message to the queue.
    - Peek: View the next message without removing it.
    - Pop: Remove and retrieve the next message.
    - Flush: Clear all messages from the queue.
- **Error Handling**: Returns error codes (`-ENOBUFS` for full queue, `-EAGAIN` for empty queue).

### Files
- `fixed_message_queue.h`: Header-only implementation.

### Usage Example
```c
#include "fixed_message_queue.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    // Declare a buffer for 10 slots of 32-byte messages
    DECLARE_MESSAGE_QUEUE_BUFFER(10, 32);
    struct message_queue mq;

    // Initialize the message queue
    message_queue_init(&mq, buffer, 32, 10);

    // Push a message
    char message[32] = "Hello, Queue!";
    if (message_queue_push(&mq, message) == 0) {
        printf("Message pushed successfully.\\n");
    } else {
        printf("Failed to push message: Queue is full.\\n");
    }

    // Peek at the message
    char peeked_message[32];
    if (message_queue_peek(&mq, peeked_message) == 0) {
        printf("Peeked message: %s\\n", peeked_message);
    } else {
        printf("Failed to peek message: Queue is empty.\\n");
    }

    // Pop the message
    char popped_message[32];
    if (message_queue_pop(&mq, popped_message) == 0) {
        printf("Popped message: %s\\n", popped_message);
    } else {
        printf("Failed to pop message: Queue is empty.\\n");
    }

    // Flush the queue
    message_queue_flush(&mq);
    printf("Queue flushed.\\n");

    return 0;
}
```

### Example Output
```
Message pushed successfully.
Peeked message: Hello, Queue!
Popped message: Hello, Queue!
Queue flushed.
```

---

## Timer Module

### Overview
The timer module provides a flexible and efficient mechanism to manage timers with customizable expiry periods and callbacks. It integrates with the priority queue to ensure timers are managed and executed based on their expiration time. Periodic timers are also supported, allowing for repetitive callbacks at a specified interval.

### Features
- **Customizable Callbacks**: Allows user-defined callbacks to be triggered upon timer expiration.
- **Periodic and One-Shot Timers**: Supports both single-use and repeating timers.
- **Integration with Priority Queue**: Uses the priority queue to efficiently handle timers based on their expiration times.
- **Efficient Bulk Reordering**: Performs reordering only when necessary to optimize performance.

### Functions

- **Initialization**:
- `void timer_module_init(void);`
    Initializes the timer module and the global timer queue.

- `void timer_init(struct timer *timer, void (*callback)(struct timer *timer, void *data), void *data);`
    Initializes a timer with a user-defined callback and optional data.

- **Timer Management**:
- `void timer_start(struct timer *timer, uint64_t ticks, bool periodic);`
    Starts or restarts a timer with a specified expiry time in ticks and periodicity.

- `void timer_stop(struct timer *timer);`
    Stops a timer and removes it from the timer queue.

- `void timer_set_period(struct timer *timer, uint64_t ticks);`
    Updates the expiry period of a timer.

- **Global Tick Handling**:
- `void timer_increment_tick(void);`
    Increments the global tick counter, processes expired timers, and reinserts periodic timers into the queue.

### Example Usage

```c
#include "timer_module.h"
#include <stdio.h>

void timer_callback(struct timer *timer, void *data) {
printf("Timer expired! Message: %s\n", (char *)data);
}

int main(void) {
struct timer t1, t2;

timer_module_init();

timer_init(&t1, timer_callback, "One-shot Timer");
timer_start(&t1, 100, false);

timer_init(&t2, timer_callback, "Periodic Timer");
timer_start(&t2, 50, true);

for (int i = 0; i < 200; i++) {
    timer_increment_tick();
}

timer_stop(&t2);

return 0;
}
```

### Example Output
```
Timer expired! Message: Periodic Timer
Timer expired! Message: One-shot Timer
Timer expired! Message: Periodic Timer
Timer expired! Message: Periodic Timer
Timer expired! Message: Periodic Timer
```

### License
This project is licensed under the MIT License. See the license details in each file.
