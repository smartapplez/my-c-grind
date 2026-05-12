#ifndef MY_STACK_H
#define MY_STACK_H

#include <stdbool.h>
#include <stddef.h>

#define DEFAULT_CAPACITY 10

typedef enum { SIZE_T, INT, DOUBLE, NONE } stack_data_type;

typedef struct c_stack c_stack;

c_stack *stack_init(stack_data_type data_type);

int stack_push(c_stack *s, const void *element);
int stack_pop(c_stack *s);
int stack_free(c_stack *s);
int stack_clear(c_stack *s);
int stack_reserve(c_stack *s, size_t new_capacity);
void *stack_top(c_stack *s);

size_t get_stack_size(c_stack *s);
size_t get_stack_capacity(c_stack *s);
size_t get_stack_elem_size(c_stack *s);
stack_data_type get_stack_data_type(c_stack *s);
bool stack_is_empty(c_stack *s);
// void print_stack(c_stack *s);
#endif
