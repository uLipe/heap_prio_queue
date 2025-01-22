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

#ifndef FIXED_MESSAGE_QUEUE_H
#define FIXED_MESSAGE_QUEUE_H

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

/**
 * Macro to round down a number to the nearest power of two at compile time.
 */
#define ROUND_DOWN_POWER_OF_TWO(x) ((x) & ~((x) - 1))

/**
 * Macro to declare a buffer suitable for the message queue.
 * @slots: Number of slots in the queue.
 * @message_size: Size of each message.
 */
#define DECLARE_MESSAGE_QUEUE_BUFFER(slots, message_size) \
    uint8_t buffer[(ROUND_DOWN_POWER_OF_TWO(slots)) * (message_size)]

/**
 * struct message_queue - Fixed-size message queue structure
 * @buffer: Pointer to the data buffer.
 * @slot_size: Size of each message in bytes.
 * @slot_count: Number of slots in the queue (power of two).
 * @head: Head index (producer).
 * @tail: Tail index (consumer).
 */
struct message_queue {
    uint8_t *buffer;
    uint32_t slot_size;
    uint32_t slot_count;
    uint32_t head;
    uint32_t tail;
};

/**
 * round_down_to_power_of_two - Round down a number to the nearest power of two
 * @value: The number to round down.
 *
 * Return: The largest power of two less than or equal to @value.
 */
static inline uint32_t round_down_to_power_of_two(uint32_t value)
{
    if (value == 0)
        return 0;

    while (value & (value - 1))
        value &= value - 1;

    return value;
}

/**
 * message_queue_init - Initialize the message queue.
 * @mq: Pointer to the message queue structure.
 * @buffer: Pointer to the buffer for storing messages.
 * @slot_size: Size of each message.
 * @slot_count: Number of slots in the buffer.
 *
 * The number of slots is rounded down to the nearest power of two.
 */
static inline void message_queue_init(struct message_queue *mq, uint8_t *buffer,
                                      uint32_t slot_size, uint32_t slot_count)
{
    mq->slot_count = round_down_to_power_of_two(slot_count);
    mq->slot_size = slot_size;
    mq->buffer = buffer;
    mq->head = 0;
    mq->tail = 0;
}

/**
 * message_queue_push - Push a message into the queue.
 * @mq: Pointer to the message queue structure.
 * @data: Pointer to the message to push.
 *
 * Return: 0 on success, -ENOBUFS if the queue is full.
 */
static inline int message_queue_push(struct message_queue *mq, const void *data)
{
    if (((mq->head + 1) & (mq->slot_count - 1)) == mq->tail) {
        return -ENOBUFS; // Queue is full
    }

    uint8_t *slot = &mq->buffer[mq->head * mq->slot_size];
    for (uint32_t i = 0; i < mq->slot_size; i++) {
        slot[i] = ((const uint8_t *)data)[i];
    }

    mq->head = (mq->head + 1) & (mq->slot_count - 1);
    return 0;
}

/**
 * message_queue_peek - Peek at the message at the front of the queue.
 * @mq: Pointer to the message queue structure.
 * @data: Pointer to store the peeked message.
 *
 * Return: 0 on success, -EAGAIN if the queue is empty.
 */
static inline int message_queue_peek(struct message_queue *mq, void *data)
{
    if (mq->head == mq->tail) {
        return -EAGAIN; // Queue is empty
    }

    uint8_t *slot = &mq->buffer[mq->tail * mq->slot_size];
    for (uint32_t i = 0; i < mq->slot_size; i++) {
        ((uint8_t *)data)[i] = slot[i];
    }

    return 0;
}

/**
 * message_queue_pop - Remove and retrieve the message at the front of the queue.
 * @mq: Pointer to the message queue structure.
 * @data: Pointer to store the popped message.
 *
 * Return: 0 on success, -EAGAIN if the queue is empty.
 */
static inline int message_queue_pop(struct message_queue *mq, void *data)
{
    int ret = message_queue_peek(mq, data);
    if (ret == 0) {
        mq->tail = (mq->tail + 1) & (mq->slot_count - 1);
    }
    return ret;
}

/**
 * message_queue_flush - Flush all messages in the queue.
 * @mq: Pointer to the message queue structure.
 */
static inline void message_queue_flush(struct message_queue *mq)
{
    mq->head = 0;
    mq->tail = 0;
}

#endif /* FIXED_MESSAGE_QUEUE_H */
