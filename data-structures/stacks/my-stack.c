#include "my-stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Time taken to program stack without much help: 2:01:57

#ifdef DEBUG
#define ERROR_PRINT(fmt, ...)                                                  \
  fprintf(stderr, "\x1b[31m[ERROR]\x1b[0m [%s:%d] - " fmt "\n", __FILE__,      \
          __func__, __LINE__, ##__VA_ARGS__)
#else
#define ERROR_PRINT(fmt, ...)
#endif

// This is has a structure of a deque but we cannot move the head pointer
struct c_stack {
  void *data;
  void *top_ptr;
  size_t capacity;
  size_t size;
  size_t elem_size;
  stack_data_type data_type;
};

c_stack *stack_init(stack_data_type data_type) {
  c_stack *init = malloc(sizeof(struct c_stack));

  switch (data_type) {
  case SIZE_T:
    init->size = 0;
    init->capacity = DEFAULT_CAPACITY;
    init->elem_size = sizeof(size_t);
    init->data_type = SIZE_T;
    init->data = malloc(sizeof(size_t) * init->capacity);
    init->top_ptr = init->data;
    break;
  case INT:
    init->size = 0;
    init->capacity = DEFAULT_CAPACITY;
    init->elem_size = sizeof(int);
    init->data_type = INT;
    init->data = malloc(sizeof(int) * init->capacity);
    init->top_ptr = init->data;
    break;
  case DOUBLE:
    init->size = 0;
    init->capacity = DEFAULT_CAPACITY;
    init->elem_size = sizeof(double);
    init->data_type = DOUBLE;
    init->data = malloc(sizeof(double) * init->capacity);
    init->top_ptr = init->data;
    break;
  case NONE:
    init->size = 0;
    init->capacity = 0;
    init->elem_size = 0;
    init->data_type = NONE;
    init->data = NULL;
    init->top_ptr = NULL;
    break;
  default:
    ERROR_PRINT("Undefined data type. Setting to NONE type");
    init->size = 0;
    init->capacity = 0;
    init->elem_size = 0;
    init->data_type = NONE;
    init->data = NULL;
    init->top_ptr = NULL;
    break;
  }
  return init;
}

// Very similar functionality to vector_pushback
int stack_push(c_stack *s, const void *element) {
  if (s == NULL) {
    ERROR_PRINT("s argument is NULL");
    return -1;
  } else if (s->data == NULL) {
    ERROR_PRINT("stack data is not initialized");
    return -1;
  } else if (s->data_type == NONE) {
    ERROR_PRINT("data type is NONE. Free and Initialize again.");
    return -1;
  }

  if (s->size == s->capacity) {
    void *temp = realloc(s->data, s->elem_size * (s->capacity << 1));
    if (temp == NULL) {
      ERROR_PRINT("Error with realloc!");
      return -1;
    }

    s->data = temp;
    s->capacity = s->capacity << 1;
  }

  char *insert_cursor = (char *)s->data + (s->elem_size * s->size);
  s->top_ptr = insert_cursor;
  memcpy(insert_cursor, element, s->elem_size);

  s->size++;
  return 0;
}

int stack_pop(c_stack *s) {
  if (s == NULL) {
    ERROR_PRINT("s argument is NULL");
    return -1;
  } else if (s->data == NULL) {
    ERROR_PRINT("stack data is not initialized");
    return -1;
  } else if (s->data_type == NONE) {
    ERROR_PRINT("data type is NONE. Free and Initialize again");
    return -1;
  } else if (s->size == 0) {
    ERROR_PRINT("stack is already empty");
    return -1;
  }

  s->size--;
  s->top_ptr -= s->elem_size;
  return 0;
}

int stack_free(c_stack *s) {
  if (s == NULL) {
    ERROR_PRINT("s argument is NULL");
    return -1;
  } else if (s->data == NULL) {
    ERROR_PRINT("stack data is already NULL");
    return -1;
  }

  free(s->data);

  // Reset metadata
  s->data = NULL;
  s->capacity = 0;
  s->size = 0;
  s->elem_size = 0;
  s->data_type = NONE;
  s->top_ptr = NULL;
  return 0;
}

int stack_clear(c_stack *s) {
  if (s == NULL) {
    ERROR_PRINT("s argument is NULL");
    return -1;
  } else if (s->data == NULL) {
    ERROR_PRINT("stack data is already NULL");
    return -1;
  } else if (s->size == 0) {
    puts("WARNING: stack is already empty");
    return -1;
  } else if (s->capacity == 0) {
    ERROR_PRINT("capacity is already zero.");
    return -1;
  }
  s->size = 0;
  s->top_ptr = s->data;

  return 0;
}

int stack_reserve(c_stack *s, size_t new_capacity) {
  if (s == NULL) {
    ERROR_PRINT("s argument is NULL");
    return -1;
  } else if (s->data == NULL) {
    ERROR_PRINT("stack data is already NULL");
    return -1;
  } else if (new_capacity <= s->size) {
    ERROR_PRINT("new capacity provided is less than the size. Memory "
                "truncation not permitted");
    return -1;
  } else if (new_capacity == 0) {
    puts("WARNING: stack_reserve(): capacity provided is 0. Freeing the stack "
         "memory");
    stack_free(s);
    return 0;
  }
  void *temp = realloc(s->data, s->elem_size * new_capacity);
  if (temp == NULL) {
    ERROR_PRINT("Error with realloc()!");
    return -1;
  }
  s->data = temp;
  s->capacity = new_capacity;
  return 0;
}

void *stack_top(c_stack *s) { return (s != NULL) ? s->top_ptr : NULL; }
size_t get_stack_size(c_stack *s) { return (s != NULL) ? s->size : 0; }
size_t get_stack_capacity(c_stack *s) { return (s != NULL) ? s->capacity : 0; }
size_t get_stack_elem_size(c_stack *s) {
  return (s != NULL) ? s->elem_size : 0;
}
stack_data_type get_stack_data_type(c_stack *s) {
  return (s != NULL) ? s->data_type : NONE;
}
bool stack_is_empty(c_stack *s) { return (s != NULL) ? (s->size == 0) : false; }
