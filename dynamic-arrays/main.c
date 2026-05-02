#include "dynamic-arrays.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void print_vector(c_vector *v) {
  for (int i = 0; i < v->size; i++) {
    int *int_pointer = vector_get(v, i);

    // NOTE: Change this according to data type
    printf("test_vector[%d] = %d\n", i, *int_pointer);
  }
}

inline void test_c_vector_no_opaque() {
  c_vector test_vector;
  vector_init(&test_vector, INT);

  int count = 0;

  for (; count < 15; count++) {
    vector_pushback(&test_vector, &count);
  }

  print_vector(&test_vector);

  for (; count < 30; count++) {
    int *ptr = vector_get(&test_vector, count - 15);
    *ptr = count;
  }

  print_vector(&test_vector);

  for (int i = 0; i < 5; i++) {
    vector_popback(&test_vector);
  }

  print_vector(&test_vector);

  size_t *illegal = vector_get(&test_vector, 9);
  if (illegal == NULL)
    puts("Successfully prevented out of bounds access\n");
  else
    printf("Out of Bounds Access: test_vector[%d] = %ld\n", 10, *illegal);

  vector_free(&test_vector);
}

int main() {
  test_c_vector_no_opaque();
  return 0;
}
