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

#ifndef INTRUSIVE_LIST_H
#define INTRUSIVE_LIST_H

#include <stdbool.h>

/**
 * struct list_node - a node in an intrusive doubly linked list
 * @prev: pointer to the previous node
 * @next: pointer to the next node
 */
struct list_node {
	struct list_node *prev;
	struct list_node *next;
};

/**
 * struct list_head - head of an intrusive doubly linked list
 * @head: pointer to the first node
 * @tail: pointer to the last node
 */
struct list_head {
	struct list_node *head;
	struct list_node *tail;
};

/**
 * list_init - Initialize a list head
 * @list: pointer to the list head
 */
static inline void list_init(struct list_head *list)
{
	list->head = NULL;
	list->tail = NULL;
}

/**
 * list_append - Append a node to the end of the list
 * @list: pointer to the list head
 * @node: pointer to the node to append
 */
static inline void list_append(struct list_head *list, struct list_node *node)
{
	node->next = NULL;
	node->prev = list->tail;

	if (list->tail)
		list->tail->next = node;
	else
		list->head = node;

	list->tail = node;
}

/**
 * list_peek_head - Peek at the head node of the list
 * @list: pointer to the list head
 *
 * Return: pointer to the head node, or NULL if the list is empty
 */
static inline struct list_node *list_peek_head(struct list_head *list)
{
	return list->head;
}

/**
 * list_prepend - Prepend a node to the beginning of the list
 * @list: pointer to the list head
 * @node: pointer to the node to prepend
 */
static inline void list_prepend(struct list_head *list, struct list_node *node)
{
	node->prev = NULL;
	node->next = list->head;

	if (list->head)
		list->head->prev = node;
	else
		list->tail = node;

	list->head = node;
}

/**
 * list_is_empty - Check if the list is empty
 * @list: pointer to the list head
 *
 * Return: true if the list is empty, false otherwise
 */
static inline bool list_is_empty(struct list_head *list)
{
	return list->head == NULL;
}

/**
 * list_insert_after - Insert a node after a given node
 * @node: pointer to the existing node
 * @new_node: pointer to the new node to insert
 */
static inline void list_insert_after(struct list_node *node, struct list_node *new_node)
{
	new_node->prev = node;
	new_node->next = node->next;

	if (node->next)
		node->next->prev = new_node;

	node->next = new_node;
}

/**
 * list_insert_before - Insert a node before a given node
 * @node: pointer to the existing node
 * @new_node: pointer to the new node to insert
 */
static inline void list_insert_before(struct list_node *node, struct list_node *new_node)
{
	new_node->next = node;
	new_node->prev = node->prev;

	if (node->prev)
		node->prev->next = new_node;

	node->prev = new_node;
}

/**
 * list_remove - Remove a node from the list
 * @node: pointer to the node to remove
 */
static inline void list_remove(struct list_node *node)
{
	if (node->prev)
		node->prev->next = node->next;

	if (node->next)
		node->next->prev = node->prev;

	node->prev = NULL;
	node->next = NULL;
}

/**
 * list_for_each_safe - Safely traverse all nodes in a list
 * @list: pointer to the list head
 * @node: pointer to the current node in the loop
 * @tmp: pointer to the next node in the loop
 */
#define list_for_each_safe(list, node, tmp)                                \
	for ((node) = (list)->head, (tmp) = (node) ? (node)->next : NULL; \
	     (node) != NULL;                                              \
	     (node) = (tmp), (tmp) = (node) ? (node)->next : NULL)

#endif /* INTRUSIVE_LIST_H */
