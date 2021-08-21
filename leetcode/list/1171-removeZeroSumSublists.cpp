#include <iostream>
#include <map>
#include <sstream>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
private:
  // prefix sum list node
  struct PSListNode {
    int sum{0};
    ListNode* node{nullptr};
    PSListNode* next{nullptr};

    PSListNode() = default;
    PSListNode(int sum, ListNode* node) : sum(sum), node(node) { }
  };

  void PrintPLList(PSListNode* head) {
    std::cout << "PSList[";
    PSListNode* cur{nullptr};
    if (head) {
      std::cout << "(" << head->sum << " " << (head->node ? head->node->val : -1000) << ")";
      cur = head->next;
    }
    while (cur) {
      std::cout << ", " << "(" << cur->sum << " " << (cur->node ? cur->node->val : -1000) << ")";
      cur = cur->next;
    }
    std::cout << "]" << std::endl;
  }

public:
  ListNode* removeZeroSumSublists(ListNode* head) {
    PSListNode* ps_head = new PSListNode();
    PSListNode* ps_cur{ps_head};
    int sum{0};
    ListNode* cur{head};
    while (cur) {
      sum += cur->val;
      ps_cur->next = new PSListNode(sum, cur);
      cur = cur->next;
      ps_cur = ps_cur->next;
    }
    ps_cur = ps_head;
    std::map<int, PSListNode*> count;
    while (ps_cur) {
      if (count.find(ps_cur->sum) == count.end()) {
	count[ps_cur->sum] = ps_cur;
      } else {
	PSListNode* prefix = count[ps_cur->sum];
	if (!prefix->node) {
	  head = ps_cur->node->next;
	} else {
	  prefix->node->next = ps_cur->node->next;
	}
	// do not forget to maintain count map
        PSListNode* n = prefix->next;
	while (n != ps_cur) {
	  count.erase(n->sum);
	  n = n->next;
	}
	// ----------------------------------
	prefix->next = ps_cur->next;
      }
      ps_cur = ps_cur->next;
    }
    return head;
  }
};

void PrintList(ListNode* head) {
  std::cout << "[";
  ListNode* cur{nullptr};
  if (head) {
    std::cout << head->val;
    cur = head->next;
  }
  while (cur) {
    std::cout << ", " << cur->val;
    cur = cur->next;
  }
  std::cout << "]" << std::endl;
}

// int main(int argc, char* argv[]) {
//   if (argc != 2) {
//     std::cerr << "ERROR INPUT" << std::endl;
//     return 1;
//   }
//   ListNode* head{nullptr};
//   ListNode* cur{nullptr};
//   std::stringstream ss(argv[1]);
//   while (true) {
//     int n{0};
//     ss >> n;
//     if (!ss) {
//       break;
//     }
//     if (!head) {
//       head = new ListNode(n);
//       cur = head;
//     } else {
//       cur->next = new ListNode(n);
//       cur = cur->next;
//     }
//   }
//   PrintList(head);
//   PrintList(Solution().removeZeroSumSublists(head));
// }
