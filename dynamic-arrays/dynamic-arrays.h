#ifndef DYNAMIC_ARRAYS_H // If MYHEADER_H is not yet defined
#define DYNAMIC_ARRAYS_H // Define it

#include <stddef.h>

#define DEFAULT_CAPACITY 10

typedef enum { SIZE_T, INT, DOUBLE, NONE } data_type;

typedef struct c_vector {
  void *data;
  size_t capacity;
  size_t size;
  size_t elem_size;
  data_type data_type;
} c_vector;

void vector_init_struct(c_vector *init, size_t init_capacity, size_t elem_size,
                        data_type type);
void vector_init(c_vector *init, data_type type);

void vector_pushback(c_vector *v, const void *element);
void *vector_get(c_vector *v, size_t index);
void vector_free(c_vector *v);
void vector_popback(c_vector *v);
#endif
