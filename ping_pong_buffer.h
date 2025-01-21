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

#ifndef PING_PONG_BUFFER_H
#define PING_PONG_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

/**
 * struct ping_pong_buffer - Lock-free ping-pong buffer structure
 * @buffers: Two buffers for double buffering
 * @size: Size of each buffer (must be identical for both)
 * @write_index: Index of the buffer currently writable by the producer
 * @read_index: Index of the buffer currently readable by the consumer
 */
struct ping_pong_buffer {
	uint8_t *buffers[2];
	uint32_t size;
	volatile uint32_t write_index;
	volatile uint32_t read_index;
};

/**
 * ping_pong_buffer_init - Initialize the ping-pong buffer
 * @ppb: Pointer to the ping-pong buffer structure
 * @buffer1: Pointer to the first buffer
 * @buffer2: Pointer to the second buffer
 * @size: Size of each buffer
 */
static inline void ping_pong_buffer_init(struct ping_pong_buffer *ppb, uint8_t *buffer1, uint8_t *buffer2, uint32_t size)
{
	ppb->buffers[0] = buffer1;
	ppb->buffers[1] = buffer2;
	ppb->size = size;
	ppb->write_index = 0;
	ppb->read_index = 1;
}

/**
 * ping_pong_buffer_write - Write data to the writable buffer
 * @ppb: Pointer to the ping-pong buffer structure
 * @data: Pointer to the data to write
 * @length: Length of the data to write
 *
 * Return: 0 on success, or -EAGAIN if the writable buffer is being read
 */
static inline int ping_pong_buffer_write(struct ping_pong_buffer *ppb, const uint8_t *data, uint32_t length)
{
	if (length > ppb->size) {
		return -EINVAL;
	}

	/* Check if the writable buffer is not being read */
	if (ppb->write_index == ppb->read_index) {
		return -EAGAIN;
	}

	/* Copy data into the writable buffer */
	uint8_t *write_buffer = ppb->buffers[ppb->write_index];
	for (uint32_t i = 0; i < length; i++) {
		write_buffer[i] = data[i];
	}

	/* Swap the buffers */
	ppb->write_index ^= 1;

	return 0;
}

/**
 * ping_pong_buffer_read - Read data from the readable buffer
 * @ppb: Pointer to the ping-pong buffer structure
 * @data: Pointer to store the read data
 * @length: Length of the data to read
 *
 * Return: 0 on success, or -EAGAIN if the readable buffer is being written to
 */
static inline int ping_pong_buffer_read(struct ping_pong_buffer *ppb, uint8_t *data, uint32_t length)
{
	if (length > ppb->size) {
		return -EINVAL;
	}

	/* Check if the readable buffer is not being written */
	if (ppb->write_index != ppb->read_index) {
		return -EAGAIN;
	}

	/* Copy data from the readable buffer */
	uint8_t *read_buffer = ppb->buffers[ppb->read_index];
	for (uint32_t i = 0; i < length; i++) {
		data[i] = read_buffer[i];
	}

	/* Swap the buffers */
	ppb->read_index ^= 1;

	return 0;
}

#endif /* PING_PONG_BUFFER_H */
