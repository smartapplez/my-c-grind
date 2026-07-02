#ifndef NODE_H
#define NODE_H

#include <stddef.h>

// Defining error codes
#define E_NODE_NULL_ARG 256
#define E_NODE_BUF_OVFLW 257

/* node supports up to 8-byte data buffer  */
typedef struct node node;

node *node_init(const void *data_in, size_t data_size);

int set_next_node(node *const node, struct node *const next_node);
int set_prev_node(node *const node, struct node *const prev_node);
node *get_next_node(const node *node);
node *get_prev_node(const node *node);

int free_node(node **node);

// I could add another parameter but the programmer is expected to use the
// get_buf_size to check on node buffer before data extraction
/* ex: if(sizeof(int) == get_buf_size(node)){
 *        cpy_from_data_buffer(node, var);
 *      }
 * Or just use one data type throughout and you'll be good
 */
int cpy_from_node_data_buffer(node *node, void *data_out, size_t data_size);

void *get_node_buffer(const node *node);

#endif // NODE_H

// Each node can technically be a different data type but that would require
// that each node have its own data type and buffer size in its metadata. Lots
// of memory to worry about there
