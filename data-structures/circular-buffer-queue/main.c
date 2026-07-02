#include "circular_buffer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static inline void test_circular_NULL_args(circular_buffer *test) {
  size_t test_buffer = 3;
  size_t test_size = circular_buffer_get_size(test);
  assert(circular_buffer_pushback(test, NULL) == E_CIRC_BUF_NULL_ARG);
  assert(circular_buffer_pushback(NULL, &test_buffer) == E_CIRC_BUF_NULL_ARG);
  assert(circular_buffer_get_size(test) == test_size);

  assert(test_size != circular_buffer_get_capacity(test));
  assert(circular_buffer_pushback(test, &test_buffer) == EXIT_SUCCESS);
  assert(circular_buffer_popfront(NULL, NULL) == E_CIRC_BUF_NULL_ARG);
  assert(circular_buffer_popfront(test, NULL) == EXIT_SUCCESS);
  assert(circular_buffer_get_size(test) == test_size);

  assert(circular_buffer_top(test, NULL) == E_CIRC_BUF_NULL_ARG);
  assert(circular_buffer_top(NULL, &test_buffer) == E_CIRC_BUF_NULL_ARG);

  assert(circular_buffer_clear(NULL) == E_CIRC_BUF_NULL_ARG);
  assert(circular_buffer_free(NULL) == E_CIRC_BUF_NULL_ARG);
}

static inline void test_circular_buffer_empty(circular_buffer *test,
                                              size_t sample_elem_size,
                                              size_t sample_capacity) {
  size_t buffer = 0;
  size_t *buffer_ptr = &buffer;
  assert(circular_buffer_popfront(test, NULL) == E_CIRC_BUF_EMPTY);
  assert(circular_buffer_clear(test) == EXIT_SUCCESS);
  assert(circular_buffer_top(test, (void *)buffer_ptr) == E_CIRC_BUF_EMPTY);
  assert(buffer == 0);
  assert(circular_buffer_get_size(test) == 0);
  assert(circular_buffer_get_capacity(test) == sample_capacity);
  assert(circular_buffer_get_elem_size(test) == sample_elem_size);
}

static inline void test_circular_buffer(void) {
  circular_buffer *test = NULL;
  size_t sample_capacity = 10;
  size_t sample_elem_size = sizeof(size_t);
  size_t test_buffer = 0;

  assert(circular_buffer_init(&test, 0, 0) == E_CIRC_BUF_INVALID_ARG);
  assert(test == NULL);
  assert(circular_buffer_init(&test, sample_elem_size, 0) ==
         E_CIRC_BUF_INVALID_ARG);
  assert(test == NULL);

  assert(circular_buffer_init(&test, sample_elem_size, sample_capacity) ==
         EXIT_SUCCESS);

  test_circular_buffer_empty(test, sample_elem_size, sample_capacity);

  // Fill up the buffer
  for (size_t i = 0; i < sample_capacity; i++) {
    assert(circular_buffer_pushback(test, &i) == EXIT_SUCCESS);
    // top or head shouldn't change, only the tail end should change
    assert(circular_buffer_top(test, &test_buffer) == EXIT_SUCCESS);
    assert(test_buffer == 0);
    assert(circular_buffer_get_size(test) == i + 1);
  }
  assert(circular_buffer_pushback(test, &sample_capacity) == E_CIRC_BUF_FULL);

  test_buffer = 0;

  // popfront the buffer to empty it
  for (size_t i = 0; i < sample_capacity; i++) {
    assert(circular_buffer_popfront(test, &test_buffer) == EXIT_SUCCESS);
    assert(circular_buffer_get_size(test) == (sample_capacity - i - 1));
    assert(test_buffer == i);
  }
  test_circular_buffer_empty(test, sample_elem_size, sample_capacity);

  test_circular_NULL_args(test);

  assert(circular_buffer_get_capacity(test) == sample_capacity);
  assert(circular_buffer_get_elem_size(test) == sample_elem_size);

  assert(circular_buffer_free(&test) == EXIT_SUCCESS);
  assert(test == NULL);
}

int main(void) {
  test_circular_buffer();
  return EXIT_SUCCESS;
}
