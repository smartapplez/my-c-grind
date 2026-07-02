#include "dynamic-arrays-opaque.h"
// #include "dynamic-arrays.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: Add a test case for reserve that decreases array capacity.
// Make sure that its size also reflects that to prevent heap OOB

// void print_vector(c_vector *v) {
// for (int i = 0; i < v->size; i++) {
// int *int_pointer = vector_get(v, i);
//
// // NOTE: Change this according to data type
// printf("test_vector[%d] = %d\n", i, *int_pointer);
// }
// }

inline void test_c_vector_no_opaque() {
  // c_vector *test_vector;
  // vector_init(&test_vector, INT);

  int count = 0;

  for (; count < 15; count++) {
    // vector_pushback(&test_vector, &count);
  }

  // print_vector(&test_vector);

  for (; count < 30; count++) {
    // int *ptr = vector_get(&test_vector, count - 15);
    // *ptr = count;
  }

  // print_vector(&test_vector);

  for (int i = 0; i < 5; i++) {
    // vector_popback(&test_vector);
  }

  // print_vector(&test_vector);

  // size_t *illegal = vector_get(&test_vector, 9);
  // if (illegal == NULL)
  // puts("Successfully prevented out of bounds access\n");
  // else
  // printf("Out of Bounds Access: test_vector[%d] = %ld\n", 10, *illegal);
  //
  // vector_free(&test_vector);
}

static inline void test_c_vector_opaque() {
  /////////////////////////
  // Unitialized test_vector tests
  int test_int = 5;
  c_vector_opaque *test_vector = NULL;
  puts("--- Testing NULL Pointer Safety ---");
  assert(test_vector == NULL); // Should be NULL

  // All operations should return error codes on NULL input
  assert(vector_pushback(test_vector, &test_int) == E_NULL_ARG);
  assert(vector_get(test_vector, 5) == NULL);
  assert(vector_free(test_vector) == E_NULL_ARG);
  assert(vector_popback(test_vector) == E_NULL_ARG);
  assert(vector_clear(test_vector) == E_NULL_ARG);
  assert(vector_set(test_vector, 5, &test_int) == E_NULL_ARG);
  assert(vector_reserve(test_vector, 10) == E_NULL_ARG);

  // Getters should return safe default values (0 or NONE) w/o crashing on NULL
  assert(vector_get_size(test_vector) == 0);
  assert(vector_get_capacity(test_vector) == 0);
  assert(vector_get_elem_size(test_vector) == 0);
  puts("Testing print vector():");
  puts("NULL pointer safety checks PASSED \n");
  /////////////////////////

  /////////////////////////
  // Initialization Test: NONE type testing
  puts("NONE type test_vector test:");
  assert(vector_pushback(test_vector, &test_int) == -1);
  assert(vector_get(test_vector, 5) == NULL);
  assert(vector_free(test_vector) == -1);
  assert(vector_popback(test_vector) == -1);
  assert(vector_clear(test_vector) == -1);
  assert(vector_set(test_vector, 5, &test_int) == -1);
  assert(vector_reserve(test_vector, 10) == -1);
  assert(vector_get_size(test_vector) == 0);
  assert(vector_get_capacity(test_vector) == 0);
  assert(vector_get_elem_size(test_vector) == 0);
  puts("Testing print vector():");
  puts("NONE type Unitialization PASSED\n");

  // Cleanup for NONE type (Don't free data, just free struct)
  vector_free(test_vector);
  free(test_vector); // Free the struct itself!
  /////////////////////////

  /////////////////////////
  // Init INT type
  test_vector = vector_init(sizeof(int));
  assert(vector_get_capacity(test_vector) == DEFAULT_CAPACITY); // Should be 10

  vector_free(test_vector);
  free(test_vector);

  puts("Initialization Tests: PASSED\n");

  printf("--- Testing Pushback & Read ---\n");
  test_vector = vector_init(sizeof(int));
  int value_to_push = 42;

  // Push one element (Size should be 1)
  assert(vector_pushback(test_vector, &value_to_push) == 0);
  assert(vector_get_size(test_vector) == 1);

  // Verify read back matches
  int *read_back_ptr = vector_get(test_vector, 0);
  int read_back = *read_back_ptr;
  assert(read_back == value_to_push);

  vector_free(test_vector);
  free(test_vector);

  puts("Pushback & Read Tests: PASSED\n");

  // Tests generated from here on was by qwen 3.5 LLM
  // More tests will follow later once I get to it
  // ==========================
  // 4. CAPACITY GROWTH TESTS
  // Ensure realloc happens when full
  // ==========================

  printf("--- Testing Capacity Growth ---\n");
  test_vector = vector_init(sizeof(int));

  // Push up to DEFAULT_CAPACITY (10 items)
  int val = 0;
  int *val_ptr = &val;
  for (int i = 0; i < 10; i++) {
    assert(vector_pushback(test_vector, val_ptr) == 0);
    val++;
  }
  assert(vector_get_capacity(test_vector) == DEFAULT_CAPACITY);

  // Push one more -> MUST trigger realloc and grow capacity
  assert(vector_pushback(test_vector, &val) == 0); // val is now 10
  size_t new_cap = vector_get_capacity(test_vector);
  assert(new_cap > DEFAULT_CAPACITY); // Capacity doubled

  vector_free(test_vector);
  free(test_vector);

  puts("Capacity Growth Tests: PASSED\n");

  // ==========================
  // 5. POPBACK & CLEAR TESTS
  // Verify we can reduce size and reset it without freeing memory
  // ==========================

  printf("--- Testing Popback & Clear ---\n");
  test_vector = vector_init(sizeof(int));

  for (int i = 0; i < 5; i++) {
    vector_pushback(test_vector, &i);
  }
  assert(vector_get_size(test_vector) == 5);

  // Pop back until empty
  assert(vector_popback(test_vector) == 0);
  assert(vector_popback(test_vector) == 0);
  assert(vector_get_size(test_vector) == 3); // I popped twice. Should be 3.

  // Let's pop the rest to test empty bounds check later
  for (int i = 0; i < 3; i++) {
    vector_popback(test_vector);
  }
  assert(vector_get_size(test_vector) == 0);

  // Now try popping from empty -> Should error
  assert(vector_popback(test_vector) == -1);

  // Test Clear (Reset size to 0, keep capacity)
  for (int i = 0; i < 5; i++) {
    vector_pushback(test_vector, &i);
  }
  assert(vector_get_size(test_vector) == 5);

  int clear_result = vector_clear(test_vector);
  // Based on your code: returns 0 if capacity > 0
  assert(clear_result == 0);
  assert(vector_get_size(test_vector) == 0);    // Size reset
  assert(vector_get_capacity(test_vector) > 0); // Capacity retained

  vector_free(test_vector);
  free(test_vector);

  puts("Popback & Clear Tests: PASSED\n");

  // ==========================
  // 6. OVERFLOW / OOB PROTECTION TESTS
  // Ensure we cannot access memory outside the current size
  // ==========================

  printf("--- Testing Out of Bounds Protection ---\n");
  test_vector = vector_init(sizeof(int));
  int dummy;

  // Push a few elements
  for (int i = 0; i < 5; i++) {
    assert(vector_pushback(test_vector, &dummy) == 0);
  }

  // Test accessing at size - 1 (Valid)
  int *get_result = vector_get(test_vector, 4);
  assert(*get_result == dummy);

  // Test accessing at size (Invalid/NULL Expected)
  void *invalid_ptr = vector_get(test_vector, 5); // Index 5 where max is 4
  assert(invalid_ptr == NULL);                    // Should fail and return NULL

  vector_free(test_vector);
  free(test_vector);

  puts("Out of Bounds Protection: PASSED\n");

  // ==========================
  // 7. RESERVED MEMORY TESTS
  // Ensure we can grow without pushing data immediately
  // ==========================

  printf("--- Testing Reserve ---\n");
  test_vector = vector_init(sizeof(int)); // Default 10 capacity

  assert(vector_reserve(test_vector, 50) == 0);
  assert(vector_get_capacity(test_vector) >= 50);

  vector_free(test_vector);
  free(test_vector);

  puts("Reserve Tests: PASSED\n");

  // ==========================
  // 8. FINAL CLEANUP CHECK
  // Verify we never leak the struct itself
  // ==========================
  printf("--- Final Cleanup Check ---\n");

  test_vector = vector_init(sizeof(int));
  for (int i = 0; i < 3; i++)
    vector_pushback(test_vector, &i);
  vector_free(test_vector); // Frees the data buffer
  free(test_vector);        // Frees the struct handle (Must not forget!)

  puts("All tests completed successfully!");
}

int main() {
  test_c_vector_opaque();
  return 0;
}
