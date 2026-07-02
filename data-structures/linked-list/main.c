#include "linked-list.h"
// #include "node.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static inline void test_linked_list() {
  // Testing NULL linked-list
  int test_val = 21;
  int test_val2 = 22;
  int test_val3 = 67;

  linked_list *list = NULL;
  assert(push_back(list, &test_val) == E_LIST_NULL_ARG);
  assert(push_front(list, &test_val) == E_LIST_NULL_ARG);
  assert(pop_back(list) == E_LIST_NULL_ARG);
  assert(pop_front(list) == E_LIST_NULL_ARG);
  assert(clear_list(list) == E_LIST_NULL_ARG);
  assert(free_list(&list) == E_LIST_NULL_ARG);
  assert(get_head_data(list) == NULL);
  assert(get_tail_data(list) == NULL);
  assert(get_data_at(list, 0) == NULL);
  assert(get_data_size(list) == 0);
  assert(get_list_size(list) == 0);

  // Testing using int
  list = linked_list_init(sizeof(int));
  assert(list != NULL);
  assert(get_data_size(list) == sizeof(int));

  // testing with empty list
  assert(pop_back(list) == E_LIST_EMPTY);
  assert(pop_front(list) == E_LIST_EMPTY);
  assert(clear_list(list) == E_LIST_EMPTY);
  assert(get_head_data(list) == NULL);
  assert(get_tail_data(list) == NULL);
  assert(get_data_at(list, 0) == NULL);
  assert(get_data_at(list, 1) == NULL);

  // Pushing to front element
  assert(push_front(list, &test_val) == EXIT_SUCCESS);
  int *data_extracted = get_data_at(list, 0);
  assert(*data_extracted == test_val);
  assert(get_data_at(list, 1) == NULL); // Out of bounds
  display_linked_list(list);
  puts("");

  assert(push_front(list, &test_val2) == 0); // test_val2 -> test_val1
  assert(push_back(list, &test_val3) ==
         0); // test_val2 -> test_val1 -> test_val3
  data_extracted = get_head_data(list);
  assert(*data_extracted == test_val2);
  data_extracted = get_tail_data(list);
  assert(*data_extracted == test_val3);
  data_extracted = get_data_at(list, 1);
  assert(*data_extracted == test_val);
  assert(get_data_at(list, 3) == NULL); // Out of bounds
  display_linked_list(list);
  puts("");

  assert(pop_back(list) == 0);
  data_extracted = get_tail_data(list);
  assert(*data_extracted == test_val);

  assert(pop_front(list) == 0);
  data_extracted = get_head_data(list);
  assert(*data_extracted == test_val);

  assert(pop_front(list) == 0);
  assert(get_head_data(list) == NULL);
  assert(get_tail_data(list) == NULL);
  assert(pop_back(list) == E_LIST_EMPTY);
  assert(pop_front(list) == E_LIST_EMPTY);

  push_front(list, &test_val3);
  push_back(list, &test_val2);
  push_back(list, &test_val);
  clear_list(list);

  free_list(&list);
}

int main() {
  test_linked_list();
  return 0;
}
