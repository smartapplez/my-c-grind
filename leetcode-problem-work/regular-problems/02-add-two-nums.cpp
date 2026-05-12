/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

#include <stdio.h>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
  ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    int carry = 0;
    struct ListNode *head_sum_node = new ListNode();
    struct ListNode *cur_sum_node = head_sum_node;

    struct ListNode *cur_node_l1 = l1;
    struct ListNode *cur_node_l2 = l2;
    while (carry > 0 || cur_node_l1 != nullptr || cur_node_l2 != nullptr) {
      const int cur_l1_val = (cur_node_l1 != nullptr) ? cur_node_l1->val : 0;
      const int cur_l2_val = (cur_node_l2 != nullptr) ? cur_node_l2->val : 0;
      const int cur_sum_digit = cur_l1_val + cur_l2_val + carry;
      carry = cur_sum_digit / 10;
      cur_sum_node->val = cur_sum_digit % 10;

      cur_node_l1 = (cur_node_l1 != nullptr) ? cur_node_l1->next : nullptr;
      cur_node_l2 = (cur_node_l2 != nullptr) ? cur_node_l2->next : nullptr;

      if (carry > 0 || cur_node_l1 != nullptr || cur_node_l2 != nullptr) {
        cur_sum_node->next = new ListNode();
        cur_sum_node = cur_sum_node->next;
      }
    }

    return head_sum_node;
  }
};

int main() {
  Solution soln;
  ListNode *l1_head = new ListNode(9, nullptr);
  ListNode *build_ptr = l1_head;
  for (int i = 0; i < 6; i++) {
    build_ptr->next = new ListNode(9, nullptr);
    build_ptr = build_ptr->next;
  }

  ListNode *l2_head = new ListNode(9, nullptr);
  build_ptr = l2_head;
  for (int i = 0; i < 3; i++) {
    build_ptr->next = new ListNode(9, nullptr);
    build_ptr = build_ptr->next;
  }

  ListNode *sum_list = soln.addTwoNumbers(l1_head, l2_head);
  ListNode *curr_node = sum_list;
  while (curr_node != nullptr) {
    printf("[%d] -> ", curr_node->val);
    curr_node = curr_node->next;
  }
  printf("NULL\n");
}
