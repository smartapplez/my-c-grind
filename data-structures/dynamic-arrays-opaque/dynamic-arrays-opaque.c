// This is technically my second version of dynamic arrays
#include "dynamic-arrays-opaque.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct c_vector_opaque {
  void *data;
  size_t capacity;
  size_t size;
  size_t elem_size;
  data_type data_type;
};

c_vector_opaque *vector_init(data_type type) {
  // Must consider the fact that the array might already have a heap
  c_vector_opaque *init = malloc(sizeof(struct c_vector_opaque));

  switch (type) {
  case SIZE_T:
    // Initialize metadata in struct
    init->size = 0;
    init->elem_size = sizeof(size_t);
    init->capacity = DEFAULT_CAPACITY;
    init->data_type = SIZE_T;

    init->data = malloc(sizeof(size_t) * init->capacity);
    break;
  case INT:
    init->size = 0;
    init->elem_size = sizeof(int);
    init->capacity = DEFAULT_CAPACITY;
    init->data_type = INT;

    init->data = malloc(sizeof(int) * init->capacity);
    break;
  case DOUBLE:
    init->size = 0;
    init->elem_size = sizeof(double);
    init->capacity = DEFAULT_CAPACITY;
    init->data_type = DOUBLE;

    init->data = malloc(sizeof(double) * init->capacity);
    break;
  case NONE:
    init->size = 0;
    init->elem_size = 0;
    init->capacity = 0;
    init->data_type = NONE;
    break;
  default:
    fprintf(stderr, "ERROR: vector_init(): data type not specified. Setting it "
                    "to NONE type\n");
    init->size = 0;
    init->elem_size = 0;
    init->capacity = 0;
    init->data_type = NONE;
    break;
  }
  return init;
}

c_vector_opaque *vector_init_struct(size_t init_capacity, size_t elem_size,
                                    data_type type) {
  c_vector_opaque *init = malloc(sizeof(struct c_vector_opaque));

  init->data = NULL;
  if (type != NONE && (init_capacity == 0)) {
    fprintf(stderr, "ERROR: vector_init_struct(): capacity are zero and data "
                    "type is NOT NONE. "
                    "Setting to NONE data_type automatically...\n");
    init->size = 0;
    init->elem_size = 0;
    init->capacity = 0;
    init->data_type = NONE;
    return init;
  }

  if (type == SIZE_T || type == INT || type == DOUBLE) {
    init->size = 0;
    init->elem_size = elem_size;
    init->capacity = init_capacity;
    init->data_type = type;
    init->data = malloc(elem_size * init_capacity);
  } else if (type == NONE) {
    init->size = 0;
    init->elem_size = 0;
    init->capacity = 0;
    init->data_type = NONE;
  } else {
    fprintf(stderr, "ERROR: vector_init_struct(): data type not specified");
    init->size = 0;
    init->elem_size = 0;
    init->capacity = 0;
    init->data_type = NONE;
  }

  return init;
}

int vector_pushback(c_vector_opaque *v, const void *element) {
  // Assume that the user pushes back the right element type
  if (v == NULL) {
    fprintf(stderr,
            "ERROR: vector_pushback(): Provided NULL to vector argument\n");
    return -1;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: vector_pushback(): vector data is NULL\n");
    return -1;
  } else if (v->data_type == NONE) {
    fprintf(stderr, "ERROR: data type of vector is NONE. Reinitialize vector "
                    "to be another data type\n");
    return -1;
  }

  // When size is equal to capacity, double the size and attempt to realloc
  if (v->size == v->capacity) {
    void *temp = realloc(v->data, v->elem_size * (v->capacity << 1));
    if (temp == NULL) {
      fprintf(stderr, "ERROR: vector_pushback(): realloc failed on v->data\n");
      return -1;
    }
    v->capacity = v->capacity << 1;
    v->data = temp;
  }

  char *insert_cursor = (char *)v->data + (v->size * v->elem_size);
  memcpy(insert_cursor, element, v->elem_size);
  v->size += 1;
  return 0;
}

int vector_popback(c_vector_opaque *v) {
  // Assume that the user pops back the right element type
  if (v == NULL) {
    fprintf(stderr,
            "ERROR: vector_popback(): Provided NULL to vector argument\n");
    return -1;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: vector_popback(): vector data is NULL\n");
    return -1;
  } else if (v->data_type == NONE) {
    fprintf(stderr, "ERROR: vector_popback(): data type of vector is NONE. "
                    "Reinitialize vector "
                    "to be another data type\n");
    return -1;
  } else if (v->size == 0) {
    fprintf(stderr, "ERROR: vector_popback(): - array is already empty\n");
    return -1;
  }

  v->size--;
  return 0;
}

void *vector_get(c_vector_opaque *v, size_t index) {
  if (v == NULL) {
    fprintf(stderr, "ERROR: vector_get(): Provided NULL to vector argument\n");
    return NULL;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: vector_get(): vector data is NULL\n");
    return NULL;
  } else if (index >= v->size) {
    fprintf(stderr, "ERROR: vector_get(): Out of bounds - index >= Size\n");
    return NULL;
  } else if (v->data_type == NONE) {
    fprintf(stderr,
            "ERROR: vector_get(): Data type of vector is set to NONE\n");
    return NULL;
  }
  // Returns the location of the element in memory
  return v->data + (index * v->elem_size);
}

int vector_free(c_vector_opaque *v) {
  if (v == NULL) {
    fprintf(stderr, "ERROR: vector_free(): c_vector_opaque pointer provided "
                    "has pointer set to NULL\n");
    return -1;
  } else if (v->data == NULL) {
    fprintf(
        stderr,
        "ERROR: vector_free(): c_vector_opaque data pointer is set to NULL\n");
    return -1;
  }

  free(v->data);

  v->data = NULL;
  v->capacity = 0;
  v->size = 0;
  v->elem_size = 0;
  v->data_type = NONE;
  return 0;
}

int vector_clear(c_vector_opaque *v) {
  if (v == NULL) {
    fprintf(stderr, "ERROR: vector_free(): Provided NULL to vector argument\n");
    return -1;
  } else if (v->size == 0) {
    puts("WARNING: vector_free(): Vector is already empty...");
    return -1;
  } else if (v->capacity == 0) {
    puts("WARNING: vector_free(): Vector's capacity is already empty...");
    return -1;
  }

  v->size = 0; // Assign size to be zero without deallocation
  return 0;
}

int vector_set(c_vector_opaque *v, size_t index, const void *element) {
  if (v == NULL) {
    fprintf(stderr, "ERROR: vector_set(): Provided NULL to vector argument\n");
    return -1;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: vector_set(): vector data is NULL\n");
    return -1;
  } else if (index >= v->size) {
    fprintf(stderr, "ERROR: vector_set(): Out of Bounds!\n");
    return -1;
  }

  char *insert_cursor = (char *)v->data + (v->elem_size * index);
  memcpy(insert_cursor, element, v->elem_size);
  return 0;
}

int vector_reserve(c_vector_opaque *v, size_t new_capacity) {
  if (v == NULL) {
    fprintf(stderr,
            "ERROR: vector_reserve(): Provided NULL to vector argument\n");
    return -1;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: vector_reserve(): vector data is NULL. Please "
                    "initialize first to initialize the metadata\n");
    return -1;
  } else if (new_capacity < v->size) {
    fprintf(stderr, "ERROR: vector_reserve(): new_capacity is less than "
                    "size.\n");
    return -1;
  }

  void *temp = realloc(v->data, v->elem_size * new_capacity);
  if (temp == NULL) {
    fprintf(stderr, "ERROR: vector_reserve(): smth went wrong with realloc()");
    return -1;
  }
  v->data = temp;
  v->capacity = new_capacity;

  return 0;
}

void print_vector(c_vector_opaque *v) {
  if (v == NULL) {
    fprintf(stderr,
            "ERROR: print_vector(): Provided NULL to vector argument\n");
    return;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: print_vector(): vector data is NULL\n");
    return;
  }

  switch (v->data_type) {
  case SIZE_T:
    for (size_t i = 0; i < v->size; i++) {
      size_t *ptr = vector_get(v, i);
      printf("v[%ld] = %ld\n", i, *ptr);
    }
    break;
  case INT:
    for (size_t i = 0; i < v->size; i++) {
      int *ptr = vector_get(v, i);
      printf("v[%ld] = %d\n", i, *ptr);
    }
    break;
  case DOUBLE:
    for (size_t i = 0; i < v->size; i++) {
      double *ptr = vector_get(v, i);
      printf("v[%ld] = %.2f\n", i, *ptr);
    }
    break;
  case NONE:
    puts("NOTE: Vector provided is NONE type.");
    break;
  }
}

// Getter functions
size_t vector_get_size(c_vector_opaque *v) { return (v != NULL) ? v->size : 0; }
size_t vector_get_capacity(c_vector_opaque *v) {
  return (v != NULL) ? v->capacity : 0;
}
size_t vector_get_elem_size(c_vector_opaque *v) {
  return (v != NULL) ? v->elem_size : 0;
}
data_type vector_get_data_type(c_vector_opaque *v) {
  return (v != NULL) ? v->data_type : NONE;
}
