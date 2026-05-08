#ifndef LINKED_LIST
#define LINKED_LIST

#include "node.h"
#include <stddef.h>

// Trading policy for more flexibility
/* Now is the programmer's responsibility to keep track of the data type of the
 * linked list instead of being restricted to a couple of data types */
typedef struct linked_list linked_list;

linked_list *linked_list_init(size_t data_size);

int push_back(linked_list *list, void *data);
int push_front(linked_list *list, void *data);
int pop_back(linked_list *list);
int pop_front(linked_list *list);
int clear_list(linked_list *list);
int free_list(linked_list *list);

void *get_head_data(linked_list *list);
void *get_tail_data(linked_list *list);
void *get_data_at(linked_list *list, size_t index);
size_t get_data_size(linked_list *list);
size_t get_list_size(linked_list *list);

void display_linked_list(linked_list *list);

#endif // !LINKED_LIST
