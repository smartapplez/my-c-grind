#include "circular_buffer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This follows a regular FIFO
// OOB -> Out Of Bounds

struct circular_buffer {
  char *head;
  char *tail;
  size_t elem_size;
  size_t size;
  size_t capacity;
  char buffer[];
};

// This fxn is a replacement for the code below since the wrap-around logic is
// the same for popfront and pushback if (cb->head == (cb->buffer +
// (cb->elem_size * (cb->capacity - 1)))) {
// cb->head = cb->buffer;
//} else {
// cb->head += cb->elem_size;
//}
static inline char *cb_advance_ptr(circular_buffer *const cb,
                                   char *const current_ptr) {
  char *end_ptr = cb->buffer + (cb->elem_size * (cb->capacity - 1));
  return (current_ptr == end_ptr) ? cb->buffer : current_ptr + cb->elem_size;
}

int circular_buffer_init(circular_buffer **circ_ptr, size_t elem_size,
                         size_t capacity) {
  if (circ_ptr == NULL)
    return E_CIRC_BUF_NULL_ARG;
  *circ_ptr = NULL;

  if (elem_size == 0 || capacity == 0) {
    return E_CIRC_BUF_INVALID_ARG;
  }

  // Prevent integer overflow on multiplication before malloc
  if (elem_size > SIZE_MAX / capacity)
    return E_CIRC_BUF_INVALID_ARG;

  // Change: one malloc instead of two different ones. Taking advantage of
  // flexible array member (C99)
  circular_buffer *init =
      malloc(sizeof(circular_buffer) + (elem_size * capacity));
  if (init == NULL) {
    // perror("circular_buffer - malloc : ");
    return E_CIRC_BUF_MALLOC;
  }

  init->elem_size = elem_size;
  init->size = 0;
  init->capacity = capacity;

  // init->buffer = malloc(elem_size * capacity);
  // if (init->buffer == NULL) {
  // perror("circular_buffer->buffer - malloc : ");
  // free(init);
  // return E_CIRC_BUF_MALLOC;
  //}
  init->head = init->buffer;
  init->tail = init->buffer;
  *circ_ptr = init;
  return EXIT_SUCCESS;
}

int circular_buffer_pushback(circular_buffer *cb, const void *elem) {
  if (cb == NULL || elem == NULL) {
    return E_CIRC_BUF_NULL_ARG;
  } else if (cb->size == cb->capacity) {
    return E_CIRC_BUF_FULL;
  }
  if (memcpy(cb->tail, elem, cb->elem_size) == NULL) {
    return E_CIRC_BUF_MEMCPY;
  }
  // Is the last pointer about to go OOB (from buffer tail)?
  cb->tail = cb_advance_ptr(cb, cb->tail);
  cb->size++;
  return EXIT_SUCCESS;
}

int circular_buffer_popfront(circular_buffer *cb, void *buffer) {
  if (cb == NULL) {
    return E_CIRC_BUF_NULL_ARG;
  } else if (cb->size == 0) {
    return E_CIRC_BUF_EMPTY;
  }

  // Copy the cb->first element to the buffer if the buffer is not NULL
  if (buffer != NULL && memcpy(buffer, cb->head, cb->elem_size) == NULL) {
    return E_CIRC_BUF_MEMCPY;
  }
  // Is the first pointer about to go OOB (from buffer tail)?
  cb->head = cb_advance_ptr(cb, cb->head);
  cb->size--;
  return EXIT_SUCCESS;
}

int circular_buffer_top(const circular_buffer *cb, void *buffer) {
  if (cb == NULL || buffer == NULL) {
    return E_CIRC_BUF_NULL_ARG;
  } else if (cb->size == 0) {
    return E_CIRC_BUF_EMPTY;
  }
  if (memcpy(buffer, cb->head, cb->elem_size) == NULL) {
    return E_CIRC_BUF_MEMCPY;
  }
  return EXIT_SUCCESS;
}

int circular_buffer_clear(circular_buffer *cb) {
  if (cb == NULL) {
    return E_CIRC_BUF_NULL_ARG;
  }
  cb->head = cb->buffer;
  cb->tail = cb->buffer;
  cb->size = 0;
  return EXIT_SUCCESS;
}

// Solving a dangling pointer because the *cb only replicates the pointer
// locally to the fxn.
int circular_buffer_free(circular_buffer **cb) {
  if (cb == NULL || *cb == NULL) {
    return E_CIRC_BUF_NULL_ARG;
  }
  // eliminated the need to free this memory area
  // free((*cb)->buffer);
  free(*cb);
  *cb = NULL;
  return EXIT_SUCCESS;
}

size_t circular_buffer_get_size(const circular_buffer *cb) {
  return (cb != NULL) ? cb->size : 0;
}
size_t circular_buffer_get_capacity(const circular_buffer *cb) {
  return (cb != NULL) ? cb->capacity : 0;
}
size_t circular_buffer_get_elem_size(const circular_buffer *cb) {
  return (cb != NULL) ? cb->elem_size : 0;
}
