#ifndef DYNAMIC_ARRAYS_OPAQUE_H // If MYHEADER_H is not yet defined
#define DYNAMIC_ARRAYS_OPAQUE_H // Define it

#include <stddef.h>

#define DEFAULT_CAPACITY 10

typedef enum { SIZE_T, INT, DOUBLE, NONE } data_type;

// Adding opaque structs to help with encapsulation
// This forces me to add in return pointers instead of helper functions
typedef struct c_vector c_vector;

c_vector *vector_init_struct(size_t init_capacity, size_t elem_size,
                             data_type type);
c_vector *vector_init(data_type type);

int vector_pushback(c_vector *v, const void *element);
void *vector_get(c_vector *v, size_t index);
int vector_free(c_vector *v);
int vector_popback(c_vector *v);
int vector_clear(c_vector *v);
int vector_set(c_vector *v, size_t index, const void *element);
int vector_reserve(c_vector *v, size_t new_capacity);

size_t get_size(c_vector *v);
size_t get_capacity(c_vector *v);
size_t get_elem_size(c_vector *v);
data_type get_data_type(c_vector *v);
void print_vector(c_vector *v);
#endif
