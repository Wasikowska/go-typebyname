#include <iostream>
#include <deque>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
  bool backtrack(ListNode* ln, TreeNode* tn) {
    if (ln->val != tn->val) {
      return false;
    }
    if (ln->next) {
      if (tn->left && backtrack(ln->next, tn->left)) {
	return true;
      }
      if (tn->right && backtrack(ln->next, tn->right)) {
	return true;
      }
      return false;
    }
    return true;
  }

  bool isSubPath(ListNode* head, TreeNode* root) {
    if (!head || !root) {
      return false;
    }
    // use bfx to find the first node
    std::deque<TreeNode*> queue;
    queue.push_back(root);
    while (!queue.empty()) {
      TreeNode* tn = queue.front();
      queue.pop_front();

      if (tn->val == head->val) {
	if (backtrack(head, tn)) {
	  return true;
	}
      }

      if (tn->left) {
	queue.push_back(tn->left);
      }
      if (tn->right) {
	queue.push_back(tn->right);
      }
    }
    return false;
  }
};

