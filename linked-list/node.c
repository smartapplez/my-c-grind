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
  char data[MAX_BUFFER_SIZE];
  node *next;
  node *prev;
};

node *node_init(void *data_in, size_t data_size) {
  node *init = malloc(sizeof(struct node));
  if (data_in == NULL) {
    ERROR_PRINT("Provided NULL to data_in argument");
    return NULL;
  } else if (data_size > MAX_BUFFER_SIZE) {
    ERROR_PRINT("data_size provided greater than MAX_BUFFER_SIZE");
    return NULL;
  }
  init->next = NULL;
  init->prev = NULL;
  memcpy(init->data, data_in, data_size);
  return init;
}

int set_next_node(node *node, struct node *next_node) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return -1;
  }
  node->next = next_node;
  return 0;
}

int set_prev_node(node *node, struct node *prev_node) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return -1;
  }
  node->prev = prev_node;
  return 0;
}

node *get_next_node(node *node) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return NULL;
  }
  return node->next;
}

node *get_prev_node(node *node) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return NULL;
  }
  return node->prev;
}

int free_node(node *node) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return NULL;
  }
  free(node);
  node = NULL;
}

int cpy_from_node_data_buffer(node *node, void *data_out, size_t data_size) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return -1;
  } else if (data_out == NULL) {
    ERROR_PRINT("Provided NULL to data_out");
    return -1;
  } else if (data_size > MAX_BUFFER_SIZE) {
    ERROR_PRINT("data_size provided is greater than MAX_BUFFER_SIZE");
    return -1;
  }
  memcpy(data_out, node->data, data_size);
  return 0;
}

void *get_node_buffer(node *node) {
  if (node == NULL) {
    ERROR_PRINT("Provided NULL to node argument");
    return NULL;
  }
  return (void *)node->data;
}
