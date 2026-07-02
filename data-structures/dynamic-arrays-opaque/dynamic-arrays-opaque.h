#ifndef DYNAMIC_ARRAYS_OPAQUE_H // If MYHEADER_H is not yet defined
#define DYNAMIC_ARRAYS_OPAQUE_H // Define it

#include <stddef.h>

// Defining error codes
#define E_NULL_ARG 258
#define E_VEC_EMPTY 259
#define E_VEC_DATA_NULL 260
#define E_OUT_OF_BOUNDS 261
#define E_REALLOC 262

#define DEFAULT_CAPACITY 10

// Adding opaque structs to help with encapsulation
// This forces me to add in return pointers instead of helper functions
typedef struct c_vector_opaque c_vector_opaque;

c_vector_opaque *vector_init(size_t elem_size);
c_vector_opaque *vector_init_struct(size_t init_capacity, size_t elem_size);

int vector_pushback(c_vector_opaque *v, const void *element);
void *vector_get(c_vector_opaque *v, size_t index);
int vector_free(c_vector_opaque *v);
int vector_popback(c_vector_opaque *v);
int vector_clear(c_vector_opaque *v);
int vector_set(c_vector_opaque *v, size_t index, const void *element);
int vector_reserve(c_vector_opaque *v, size_t new_capacity);

size_t vector_get_size(c_vector_opaque *v);
size_t vector_get_capacity(c_vector_opaque *v);
size_t vector_get_elem_size(c_vector_opaque *v);
// void print_vector(c_vector_opaque *v);
#endif
