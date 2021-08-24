#include <iostream>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
  ListNode* rotateRight(ListNode* head, int k) {
    if (!head || !head->next) {
      return head;
    }
    ListNode* n1{head};
    ListNode* n2{head};
    int len{1};
    while (true) {
      if (!n1->next) {
	k = k % len;
      }

      if (k == 0) {
	break;
      }

      n1 = n1->next ? n1->next : head;
      len++;
      k--;
    }

    while (n1->next) {
      n1 = n1->next;
      n2 = n2->next;
    }

    n1->next = head;
    head = n2->next;
    n2->next = nullptr;

    return head;
  }
};
