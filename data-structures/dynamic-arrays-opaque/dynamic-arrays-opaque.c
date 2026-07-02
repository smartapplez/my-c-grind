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
};

c_vector_opaque *vector_init(size_t elem_size) {
  if (elem_size == 0) {
    fprintf(stderr, "ERROR: vector_init(): elem_size is zero\n");
    return NULL;
  }
  c_vector_opaque *init = malloc(sizeof(c_vector_opaque));
  if (init == NULL) {
    perror("c_vector - malloc error on init : ");
    return NULL;
  }

  init->size = 0;
  init->elem_size = elem_size;
  init->capacity = DEFAULT_CAPACITY;

  init->data = malloc(sizeof(elem_size) * DEFAULT_CAPACITY);
  if (init->data == NULL) {
    free(init);
    init = NULL;
    perror("c_vector - malloc error on init->data : ");
  }
  return init;
}

c_vector_opaque *vector_init_struct(size_t init_capacity, size_t elem_size) {
  if (init_capacity == 0 || elem_size == 0) {
    fprintf(stderr,
            "ERROR: vector_init_struct(): capacity or elem_size is zero\n");
    return NULL;
  }
  c_vector_opaque *init = malloc(sizeof(struct c_vector_opaque));
  if (init == NULL) {
    perror("c_vector - malloc error on init : ");
    return NULL;
  }

  init->size = 0;
  init->elem_size = elem_size;
  init->capacity = init_capacity;

  init->data = malloc(sizeof(elem_size) * init_capacity);
  if (init->data == NULL) {
    free(init);
    init = NULL;
    perror("c_vector - malloc error on init->data : ");
  }
  return init;
}

int vector_pushback(c_vector_opaque *v, const void *element) {
  // Assume that the user pushes back the right element type
  if (v == NULL) {
    fprintf(stderr,
            "ERROR: vector_pushback(): Provided NULL to vector argument\n");
    return E_NULL_ARG;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: vector_pushback(): vector data is NULL\n");
    return E_VEC_DATA_NULL;
  }

  // When size is equal to capacity, double the size and attempt to realloc
  if (v->size == v->capacity) {
    void *temp = realloc(v->data, v->elem_size * (v->capacity << 1));
    if (temp == NULL) {
      perror("c_vector - resize error on realloc() : ");
      return E_REALLOC;
    }
    v->capacity = v->capacity << 1;
    v->data = temp;
  }

  char *insert_cursor = (char *)v->data + (v->size * v->elem_size);
  memcpy(insert_cursor, element, v->elem_size);
  v->size += 1;
  return EXIT_SUCCESS;
}

int vector_popback(c_vector_opaque *v) {
  // Assume that the user pops back the right element type
  if (v == NULL) {
    fprintf(stderr,
            "ERROR: vector_popback(): Provided NULL to vector argument\n");
    return E_NULL_ARG;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: vector_popback(): vector data is NULL\n");
    return E_VEC_DATA_NULL;
  } else if (v->size == 0) {
    fprintf(stderr, "ERROR: vector_popback(): - array is already empty\n");
    return E_VEC_EMPTY;
  }

  v->size--;
  return EXIT_SUCCESS;
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
  }
  // Returns the location of the element in memory
  return v->data + (index * v->elem_size);
}

int vector_free(c_vector_opaque *v) {
  if (v == NULL) {
    fprintf(stderr, "ERROR: vector_free(): c_vector_opaque pointer provided "
                    "has pointer set to NULL\n");
    return E_NULL_ARG;
  } else if (v->data == NULL) {
    fprintf(
        stderr,
        "ERROR: vector_free(): c_vector_opaque data pointer is set to NULL\n");
    return E_VEC_DATA_NULL;
  }

  free(v->data);

  v->data = NULL;
  v->capacity = 0;
  v->size = 0;
  v->elem_size = 0;
  return EXIT_SUCCESS;
}

int vector_clear(c_vector_opaque *v) {
  if (v == NULL) {
    fprintf(stderr, "ERROR: vector_free(): Provided NULL to vector argument\n");
    return E_NULL_ARG;
  } else if (v->size == 0) {
    puts("WARNING: vector_free(): Vector is already empty...");
    return E_VEC_EMPTY;
  } else if (v->capacity == 0) {
    puts("WARNING: vector_free(): Vector's capacity is already empty...");
    return E_VEC_EMPTY;
  }

  v->size = 0; // Assign size to be zero without deallocation
  return EXIT_SUCCESS;
}

int vector_set(c_vector_opaque *v, size_t index, const void *element) {
  if (v == NULL) {
    fprintf(stderr, "ERROR: vector_set(): Provided NULL to vector argument\n");
    return E_NULL_ARG;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: vector_set(): vector data is NULL\n");
    return E_VEC_DATA_NULL;
  } else if (index >= v->size) {
    fprintf(stderr, "ERROR: vector_set(): Out of Bounds!\n");
    return E_OUT_OF_BOUNDS;
  }

  char *insert_cursor = (char *)v->data + (v->elem_size * index);
  memcpy(insert_cursor, element, v->elem_size);
  return EXIT_SUCCESS;
}

// FIX: You can just easily break this function LMAO
int vector_reserve(c_vector_opaque *v, size_t new_capacity) {
  if (v == NULL) {
    fprintf(stderr,
            "ERROR: vector_reserve(): Provided NULL to vector argument\n");
    return E_NULL_ARG;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: vector_reserve(): vector data is NULL. Please "
                    "initialize first to initialize the metadata\n");
    return E_VEC_DATA_NULL;
  } else if (new_capacity < v->size) {
    fprintf(stderr, "ERROR: vector_reserve(): new_capacity is less than "
                    "size.\n");
    return -1;
  }

  void *temp = realloc(v->data, v->elem_size * new_capacity);
  if (temp == NULL) {
    perror("c_vector - resize error on realloc() : ");
    return E_REALLOC;
  }
  v->data = temp;
  v->capacity = new_capacity;

  return EXIT_SUCCESS;
}

void print_vector(c_vector_opaque *v) {
  // Unsupported for now until I know how to easily integrate different flags

  if (v == NULL) {
    fprintf(stderr,
            "ERROR: print_vector(): Provided NULL to vector argument\n");
    return;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: print_vector(): vector data is NULL\n");
    return;
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
