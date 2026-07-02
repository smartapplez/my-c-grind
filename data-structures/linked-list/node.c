#include "node.h"
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#define ERROR_PRINT(fmt, ...)                                                  \
  fprintf(stderr, "\x1b[31m[ERROR]\x1b[0m [%s:%d] - " fmt "\n", __FILE__,      \
          __func__, __LINE__, ##__VA_ARGS__)
#else
#define ERROR_PRINT(fmt, ...)
#endif

// supports up to 8 bytes
// Ik this is wasteful when it comes to ints, but this is for performance
// reasons lol
struct node {
  node *next;
  node *prev;
  char data[];
};

node *node_init(const void *data_in, size_t data_size) {
  node *init = malloc(sizeof(struct node) + data_size);
  if (data_in == NULL) {
    ERROR_PRINT("Provided NULL to data_in argument");
    return NULL;
  } else if (data_size == 0) {
    ERROR_PRINT("data_size provided is zero");
    return NULL;
  }

  init->next = NULL;
  init->prev = NULL;
  memcpy(init->data, data_in, data_size);
  return init;
}

int set_next_node(node *const node, struct node *const next_node) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return E_NODE_NULL_ARG;
  }
  node->next = next_node;
  return EXIT_SUCCESS;
}

int set_prev_node(node *const node, struct node *const prev_node) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return E_NODE_NULL_ARG;
  }
  node->prev = prev_node;
  return EXIT_SUCCESS;
}

node *get_next_node(const node *node) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return NULL;
  }
  return node->next;
}

node *get_prev_node(const node *node) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return NULL;
  }
  return node->prev;
}

int free_node(node **node) {
  if (node == NULL || (*node) == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return E_NODE_NULL_ARG;
  }
  free((*node));
  node = NULL;
  return EXIT_SUCCESS;
}

int cpy_from_node_data_buffer(node *node, void *data_out, size_t data_size) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return E_NODE_NULL_ARG;
  } else if (data_out == NULL) {
    ERROR_PRINT("Provided NULL to data_out");
    return E_NODE_NULL_ARG;
  }
  memcpy(data_out, node->data, data_size);
  return EXIT_SUCCESS;
}

void *get_node_buffer(const node *node) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return NULL;
  }
  return (void *)node->data;
}
