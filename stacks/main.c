#include "my-stack.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static inline void test_stack() {
  /////////////////////////
  // Testing for NULL
  c_stack *test_stack = NULL;
  int test_int = 5;
  puts("--- Testing NULL Pointer Safety ---");
  assert(stack_push(test_stack, &test_int) == -1);
  assert(stack_pop(test_stack) == -1);
  assert(stack_free(test_stack) == -1);
  assert(stack_clear(test_stack) == -1);
  assert(stack_reserve(test_stack, 15) == -1);
  assert(stack_top(test_stack) == NULL);

  // Getters should return safe default values (0 or NONE) w/o crashing on NULL
  assert(get_stack_size(test_stack) == 0);
  assert(get_stack_capacity(test_stack) == 0);
  assert(get_stack_elem_size(test_stack) == 0);
  assert(get_stack_data_type(test_stack) == NONE);
  assert(~stack_is_empty(test_stack));
  /////////////////////////

  /////////////////////////
  // Initialization Test: NONE type testing
  // TODO: Add more test cases here
  // they should be similar to the dynamic arrays testing
}

int main() {
  test_stack();
  return 0;
}
