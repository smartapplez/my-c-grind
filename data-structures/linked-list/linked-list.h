#ifndef LINKED_LIST
#define LINKED_LIST

#include "node.h"
#include <stddef.h>
#include <stdlib.h>

// Defining ERROR Macros:
#define E_LIST_MAX_SIZE_OVERFLOW 257
#define E_LIST_NULL_ARG 258
#define E_LIST_EMPTY 259

// Trading policy for more flexibility
/* Now is the programmer's responsibility to keep track of the data type of the
 * linked list instead of being restricted to a couple of data types */
typedef struct linked_list linked_list;

linked_list *linked_list_init(const size_t data_size);

int push_back(linked_list *list, const void *data);
int push_front(linked_list *list, const void *data);
int pop_back(linked_list *list);
int pop_front(linked_list *list);
int clear_list(linked_list *list);
int free_list(linked_list **list);

void *get_head_data(const linked_list *list);
void *get_tail_data(const linked_list *list);
void *get_data_at(const linked_list *list, const size_t index);
size_t get_data_size(const linked_list *list);
size_t get_list_size(const linked_list *list);

void display_linked_list(const linked_list *list);

#endif // !LINKED_LIST
