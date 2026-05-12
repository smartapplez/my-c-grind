#ifndef NODE_H
#define NODE_H

#include <stddef.h>

/* node supports up to 8-byte data buffer  */
typedef struct node node;

/* Defines the maximum buffer size in bytes */
#define MAX_BUFFER_SIZE 8

node *node_init(void *data_in, size_t data_size);

int set_next_node(node *node, struct node *next_node);
int set_prev_node(node *node, struct node *prev_node);
node *get_next_node(node *node);
node *get_prev_node(node *node);

int free_node(node *node);

// I could add another parameter but the programmer is expected to use the
// get_buf_size to check on node buffer before data extraction
/* ex: if(sizeof(int) == get_buf_size(node)){
 *        cpy_from_data_buffer(node, var);
 *      }
 * Or just use one data type throughout and you'll be good
 */
int cpy_from_node_data_buffer(node *node, void *data_out, size_t data_size);

void *get_node_buffer(node *node);

#endif // NODE_H

// Each node can technically be a different data type but that would require
// that each node have its own data type and buffer size in its metadata. Lots
// of memory to worry about there
