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

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

/**
 * struct ring_buffer - Fast ring buffer structure
 * @buffer: pointer to the data buffer
 * @size: size of the buffer (must be a power of two)
 * @head: head index (producer)
 * @tail: tail index (consumer)
 */
struct ring_buffer {
	uint8_t *buffer;
	uint32_t size;
	uint32_t head;
	uint32_t tail;
};

/**
 * round_down_to_power_of_two - Round down a number to the nearest power of two
 * @value: the number to round down
 *
 * Return: the largest power of two less than or equal to @value
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
 * ring_buffer_init - Initialize the ring buffer
 * @rb: pointer to the ring buffer structure
 * @buffer: pointer to the data buffer
 * @size: size of the data buffer
 */
static inline void ring_buffer_init(struct ring_buffer *rb, uint8_t *buffer, uint32_t size)
{
	rb->size = round_down_to_power_of_two(size);
	rb->buffer = buffer;
	rb->head = 0;
	rb->tail = 0;
}

/**
 * ring_buffer_is_full - Check if the ring buffer is full
 * @rb: pointer to the ring buffer structure
 *
 * Return: true if the buffer is full, false otherwise
 */
static inline bool ring_buffer_is_full(struct ring_buffer *rb)
{
	return ((rb->head + 1) & (rb->size - 1)) == rb->tail;
}

/**
 * ring_buffer_is_empty - Check if the ring buffer is empty
 * @rb: pointer to the ring buffer structure
 *
 * Return: true if the buffer is empty, false otherwise
 */
static inline bool ring_buffer_is_empty(struct ring_buffer *rb)
{
	return rb->head == rb->tail;
}

/**
 * ring_buffer_push - Push a byte into the ring buffer
 * @rb: pointer to the ring buffer structure
 * @byte: the byte to push
 *
 * Return: 0 on success, or -ENOBUFS if the buffer is full
 */
static inline int ring_buffer_push(struct ring_buffer *rb, uint8_t byte)
{
	if (ring_buffer_is_full(rb)) {
		return -ENOBUFS;
	}

	rb->buffer[rb->head] = byte;
	rb->head = (rb->head + 1) & (rb->size - 1);

	return 0;
}

/**
 * ring_buffer_pop - Pop a byte from the ring buffer
 * @rb: pointer to the ring buffer structure
 * @byte: pointer to store the popped byte
 *
 * Return: 0 on success, or -EAGAIN if the buffer is empty
 */
static inline int ring_buffer_pop(struct ring_buffer *rb, uint8_t *byte)
{
	if (ring_buffer_is_empty(rb)) {
		return -EAGAIN;
	}

	*byte = rb->buffer[rb->tail];
	rb->tail = (rb->tail + 1) & (rb->size - 1);

	return 0;
}

/**
 * ring_buffer_copy_from_stream - Copy bytes from a stream into the ring buffer
 * @rb: pointer to the ring buffer structure
 * @stream: pointer to the source stream
 * @length: number of bytes to copy
 *
 * Return: number of bytes copied on success, or -ENOBUFS if insufficient space
 */
static inline int ring_buffer_copy_from_stream(struct ring_buffer *rb, const uint8_t *stream, uint32_t length)
{
	uint32_t free_space = (rb->size - ((rb->head - rb->tail) & (rb->size - 1))) - 1;

	if (length > free_space) {
		return -ENOBUFS;
	}

	for (uint32_t i = 0; i < length; i++) {
		rb->buffer[rb->head] = stream[i];
		rb->head = (rb->head + 1) & (rb->size - 1);
	}

	return length;
}

#endif /* RING_BUFFER_H */
