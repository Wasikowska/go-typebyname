#include <iostream>
#include <unordered_set>

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
  ListNode *detectCycle(ListNode *head) {
    if (!head) {
      return nullptr;
    }
    // floyd's cycle finding algorithm
    ListNode* n1{head};
    ListNode* n2{head->next};

    while (n1 && n2) {
      if (n1 == n2) {
	// find a cycle
	std::unordered_set<ListNode*> cycle;
	while (n1) {
	  if (cycle.find(n1) != cycle.end()) {
	    break;
	  }
	  cycle.insert(n1);
	  n1 = n1->next;
	}

	ListNode* n{head};
	while (n) {
	  if (cycle.find(n) != cycle.end()) {
	    return n;
	  }
	  n = n->next;
	}
      }

      n1 = n1->next;
      n2 = n2->next ? n2->next->next : nullptr;
    }

    return nullptr;
  }
};
