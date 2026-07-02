#include "linked-list.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifdef DEBUG
#define ERROR_PRINT(fmt, ...)                                                  \
  fprintf(stderr, "\x1b[31m[ERROR]\x1b[0m [%s:%d] - " fmt "\n", __FILE__,      \
          __func__, __LINE__, ##__VA_ARGS__)
#else
#define ERROR_PRINT(fmt, ...)
#endif

struct linked_list {
  node *head;
  node *tail;
  size_t data_size;
  size_t list_size;
};

linked_list *linked_list_init(const size_t data_size) {
  if (data_size == 0) {
    ERROR_PRINT("data_size provided is zero");
    return NULL;
  }
  linked_list *init = malloc(sizeof(struct linked_list));
  if (init == NULL) {
    ERROR_PRINT("error with malloc");
    return NULL;
  }

  init->head = NULL;
  init->tail = NULL;
  init->data_size = data_size;
  init->list_size = 0;
  return init;
}

int push_back(linked_list *list, const void *data) {
  if (list == NULL) {
    ERROR_PRINT("Provided NULL to list argument");
    return E_LIST_NULL_ARG;
  } else if (data == NULL) {
    ERROR_PRINT("Provided NULL to data argument");
    return E_LIST_NULL_ARG;
  }

  if (list->tail != NULL) {
    node *new_node = node_init(data, list->data_size);
    set_next_node(list->tail, new_node); // cur_tail -> new_tail
    set_prev_node(new_node, list->tail); // cur_tail <- new_tail
    list->tail = new_node;
  } else { // In the case of an empty list
    list->head = node_init(data, list->data_size);
    list->tail = list->head;
  }
  list->list_size++;
  return EXIT_SUCCESS;
}

int push_front(linked_list *list, const void *data) {
  if (list == NULL) {
    ERROR_PRINT("Provided NULL to list argument");
    return E_LIST_NULL_ARG;
  } else if (data == NULL) {
    ERROR_PRINT("Provided NULL to data argument");
    return E_LIST_NULL_ARG;
  }

  // In the case of an empty list
  if (list->head != NULL) {
    node *new_node = node_init(data, list->data_size);
    set_next_node(new_node, list->head); // new_head -> cur_head
    set_prev_node(list->head, new_node); // new_head <- cur_head
    list->head = new_node;
  } else {
    list->head = node_init(data, list->data_size);
    list->tail = list->head;
  }
  list->list_size++;
  return EXIT_SUCCESS;
}

int pop_back(linked_list *list) {
  if (list == NULL) {
    ERROR_PRINT("Provided NULL to list argument");
    return E_LIST_NULL_ARG;
  } else if (list->tail == NULL) {
    ERROR_PRINT("list is already empty");
    return E_LIST_EMPTY;
  }
  node *new_tail = get_prev_node(list->tail);
  if (new_tail != NULL)
    set_next_node(new_tail, NULL);
  free_node(&list->tail);
  list->tail = new_tail;
  list->head = (list->tail != NULL) ? list->head : NULL;
  list->list_size--;
  return EXIT_SUCCESS;
}

int pop_front(linked_list *list) {
  if (list == NULL) {
    ERROR_PRINT("Provided NULL to list argument");
    return E_LIST_NULL_ARG;
  } else if (list->head == NULL) {
    ERROR_PRINT("list is already empty");
    return E_LIST_EMPTY;
  }
  node *new_head = get_next_node(list->head);
  if (new_head != NULL)
    set_prev_node(new_head, NULL);
  free_node(&list->head);
  list->head = new_head;
  list->tail = (list->head != NULL) ? list->tail : NULL;
  list->list_size--;
  return EXIT_SUCCESS;
}

int clear_list(linked_list *list) {
  if (list == NULL) {
    ERROR_PRINT("Provided NULL to list argument");
    return E_LIST_NULL_ARG;
  } else if (list->head == NULL && list->tail == NULL) {
    ERROR_PRINT("List is already empty");
    return E_LIST_EMPTY;
  }

  node *cur_node = list->head;
  node *next_node;

  while (cur_node != NULL) {
    next_node = get_next_node(cur_node);
    free_node(&cur_node);
    cur_node = next_node;
  }

  list->head = NULL;
  list->tail = NULL;
  list->list_size = 0;
  return EXIT_SUCCESS;
}

int free_list(linked_list **list) {
  if (list == NULL || (*list) == NULL) {
    ERROR_PRINT("Provided NULL to list argument");
    return E_LIST_NULL_ARG;
  }
  node *cur_node = (*list)->head;
  node *next_node;

  while (cur_node != NULL) {
    next_node = get_next_node(cur_node);
    free_node(&cur_node);
    cur_node = next_node;
  }
  free((*list));
  *list = NULL;
  return EXIT_SUCCESS;
}

void *get_head_data(const linked_list *list) {
  if (list == NULL) {
    ERROR_PRINT("Provided NULL to list argument");
    return NULL;
  } else if (list->head == NULL) {
    ERROR_PRINT("List head is NULL");
    return NULL;
  }
  return get_node_buffer(list->head);
}

void *get_tail_data(const linked_list *list) {
  if (list == NULL) {
    ERROR_PRINT("Provided NULL to list argument");
    return NULL;
  } else if (list->tail == NULL) {
    ERROR_PRINT("List tail is NULL");
    return NULL;
  }
  return get_node_buffer(list->tail);
}

void *get_data_at(const linked_list *list, const size_t index) {
  if (list == NULL) {
    ERROR_PRINT("Provided NULL to list argument");
    return NULL;
  } else if (index >= list->list_size) {
    ERROR_PRINT("OUT OF BOUNDS: Provided index is bigger than list_size");
    return NULL;
  }
  node *cur_node = list->head;
  for (size_t i = 0; i < index; i++) {
    cur_node = get_next_node(cur_node);
  }
  return get_node_buffer(cur_node);
}

size_t get_data_size(const linked_list *list) {
  return (list != NULL) ? list->data_size : 0;
}

size_t get_list_size(const linked_list *list) {
  return (list != NULL) ? list->list_size : 0;
}

// I have to idea how to adjust the display function for bigger buffer sizes
// This is by no means reliable since in ints, they will result in uninitialised
// values
void display_linked_list(const linked_list *list) {
  if (list == NULL) {
    ERROR_PRINT("Provided NULL to list argument");
    return;
  } else if (list->head == NULL) {
    puts("List is empty...");
    return;
  }
  node *cur_node = list->head;
  puts("Linked List (in Quadword Hex):");

  while (cur_node != NULL) {
    u_int64_t quadword;
    // Safest way in case bytes are not 8-bits
    memcpy(&quadword, get_node_buffer(cur_node), sizeof(u_int64_t));
    printf("%016llx ->\n", (unsigned long long)quadword);
    cur_node = get_next_node(cur_node);
  }
  puts("NULL");
}
