#include <stdio.h>
#include <stdlib.h>
/*
 * NOTE: To use hashes in C, you can use either use GLib, uthash, or maybe
 * liburcu all can be installed through my package manager
 */

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode {
  int val;
  struct ListNode *next;
};

static inline void construct_listNode(struct ListNode *node, int val) {
  node->next = NULL;
  node->val = val;
}

struct ListNode *addTwoNumbers(struct ListNode *l1, struct ListNode *l2) {
  int carry = 0;
  struct ListNode *head_sum_node = malloc(sizeof(struct ListNode));
  construct_listNode(head_sum_node, 0);
  struct ListNode *cur_sum_node = head_sum_node;

  struct ListNode *cur_node_l1 = l1;
  struct ListNode *cur_node_l2 = l2;
  while (carry || cur_node_l1 != NULL || cur_node_l2 != NULL) {
    const int cur_l1_val = (cur_node_l1 != NULL) ? cur_node_l1->val : 0;
    const int cur_l2_val = (cur_node_l2 != NULL) ? cur_node_l2->val : 0;
    const int cur_sum_digit = cur_l1_val + cur_l2_val + carry;
    carry = cur_sum_digit / 10;
    cur_sum_node->val = cur_sum_digit % 10;

    cur_node_l1 = (cur_node_l1 != NULL) ? cur_node_l1->next : NULL;
    cur_node_l2 = (cur_node_l2 != NULL) ? cur_node_l2->next : NULL;

    if (carry > 0 || cur_node_l1 != NULL || cur_node_l2 != NULL) {
      cur_sum_node->next = malloc(sizeof(struct ListNode));
      construct_listNode(cur_sum_node->next, 0);
      cur_sum_node = cur_sum_node->next;
    }
  }
  return head_sum_node;
}

void free_ListNode(struct ListNode *head_node) {
  struct ListNode *cur_node = head_node;
  struct ListNode *next_node;

  while (cur_node != NULL) {
    next_node = cur_node->next;
    free(cur_node);
    cur_node = next_node;
  }
  head_node = NULL;
}

int main() {
  const unsigned long listNode_size = sizeof(struct ListNode);
  struct ListNode *l1_head = malloc(listNode_size);
  struct ListNode *build_ptr = l1_head;
  construct_listNode(l1_head, 9);

  for (int i = 0; i < 6; i++) {
    build_ptr->next = malloc(listNode_size);
    construct_listNode(build_ptr->next, 9);
    build_ptr = build_ptr->next;
  }

  struct ListNode *l2_head = malloc(listNode_size);
  build_ptr = l2_head;
  construct_listNode(l2_head, 9);
  for (int i = 0; i < 3; i++) {
    build_ptr->next = malloc(listNode_size);
    construct_listNode(build_ptr->next, 9);
    build_ptr = build_ptr->next;
  }

  struct ListNode *sum_list = addTwoNumbers(l1_head, l2_head);
  struct ListNode *curr_node = sum_list;
  while (curr_node != NULL) {
    printf("[%d] -> ", curr_node->val);
    curr_node = curr_node->next;
  }
  printf("NULL\n");
  free_ListNode(l1_head);
  free_ListNode(l2_head);
  free_ListNode(sum_list);
}
