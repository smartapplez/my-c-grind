#include "dynamic-arrays.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vector_init(c_vector *init, data_type type) {
  // Must consider the fact that the array might already have a heap
  if (init == NULL) {
    fprintf(stderr, "ERROR: init is NULL. Declare it first and then pass it as "
                    "reference\n");
    return;
  }
  // TODO: Determmine how to free the pointer at a init->v if there is already
  // an existing array

  init->data = NULL;

  if (type == NONE) {
    init->size = 0;
    init->elem_size = 0;
    init->capacity = 0;
    init->data_type = NONE;
    return;
  }

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
    fprintf(stderr,
            "ERROR: data type not specified. Setting it to NONE type\n");
    init->size = 0;
    init->elem_size = 0;
    init->capacity = 0;
    init->data_type = NONE;
    break;
  }
}

void vector_init_struct(c_vector *init, size_t init_capacity, size_t elem_size,
                        data_type type) {
  init->data = NULL;

  if (type != NONE && (init_capacity == 0)) {
    fprintf(stderr, "capacity are zero and data type is NOT NONE. "
                    "Setting to NONE data_type automatically...\n");
    init->size = 0;
    init->elem_size = 0;
    init->capacity = 0;
    init->data_type = NONE;
    return;
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
    fprintf(stderr, "data type not specified");
    init->size = 0;
    init->elem_size = 0;
    init->capacity = 0;
    init->data_type = NONE;
  }
}

void vector_pushback(c_vector *v, const void *element) {
  // Assume that the user pushes back the right element type
  if (v == NULL) {
    fprintf(stderr, "ERROR: Provided NULL to vector argument\n");
    return;
  } else if (v->data_type == NONE) {
    fprintf(stderr, "ERROR: data type of vector is NONE. Reinitialize vector "
                    "to be another data type\n");
    return;
  }

  if (v->size == v->capacity) {
    v->capacity = v->capacity << 1;

    void *temp = realloc(v->data, v->elem_size * v->capacity);
    if (temp == NULL) {
      fprintf(stderr, "ERROR: realloc failed on v->data\n");
      return;
    }

    v->data = temp;
  }

  char *insert_cursor = (char *)v->data + (v->size * v->elem_size);
  memcpy(insert_cursor, element, v->elem_size);
  v->size += 1;
}

void vector_popback(c_vector *v) {
  // Assume that the user pops back the right element type
  if (v == NULL) {
    fprintf(stderr, "ERROR: Provided NULL to vector argument\n");
    return;
  } else if (v->data_type == NONE) {
    fprintf(stderr, "ERROR: data type of vector is NONE. Reinitialize vector "
                    "to be another data type\n");
    return;
  } else if (v->size == 0) {
    fprintf(stderr, "ERROR: pop_back() - array is already empty\n");
    return;
  }

  v->size--;
}

void *vector_get(c_vector *v, size_t index) {
  if (v == NULL) {
    fprintf(stderr, "ERROR: Provided NULL to vector argument\n");
    return NULL;
  } else if (index >= v->size) {
    fprintf(stderr, "ERROR: Out of bounds - index >= Size\n");
    return NULL;
  } else if (v->data_type == NONE) {
    fprintf(stderr, "ERROR: Data type of vector is set to NONE\n");
    return NULL;
  }
  // Returns the location of the element in memory
  return v->data + (index * v->elem_size);
}

void vector_free(c_vector *v) {
  if (v == NULL) {
    fprintf(stderr,
            "ERROR: c_vector pointer provided has pointer set to NULL\n");
    return;
  } else if (v->data == NULL) {
    fprintf(stderr, "ERROR: c_vector data pointer is set to NULL\n");
    return;
  }

  free(v->data);

  v->data = NULL;
  v->capacity = 0;
  v->size = 0;
  v->elem_size = 0;
  v->data_type = NONE;
}
