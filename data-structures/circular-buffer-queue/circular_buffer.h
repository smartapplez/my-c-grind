#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdlib.h>

// Defining error codes
#define E_CIRC_BUF_NULL_ARG 258
#define E_CIRC_BUF_EMPTY 259
#define E_CIRC_BUF_MEMCPY 260
#define E_CIRC_BUF_INVALID_ARG 261
#define E_CIRC_BUF_MALLOC 262
#define E_CIRC_BUF_FULL 263

typedef struct circular_buffer circular_buffer;

int circular_buffer_init(circular_buffer **circ_ptr, size_t data_size,
                         size_t capacity);

int circular_buffer_pushback(circular_buffer *cb, const void *elem);
int circular_buffer_popfront(circular_buffer *cb, void *buffer);
int circular_buffer_top(const circular_buffer *cb, void *buffer);
int circular_buffer_clear(circular_buffer *cb);
int circular_buffer_free(circular_buffer **cb);

size_t circular_buffer_get_size(const circular_buffer *cb);
size_t circular_buffer_get_capacity(const circular_buffer *cb);
size_t circular_buffer_get_elem_size(const circular_buffer *cb);

#endif // !CIRCULAR_BUFFER_H
