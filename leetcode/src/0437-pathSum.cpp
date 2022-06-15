#include <deque>

using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
private:
  deque<TreeNode*> path;
  long sum{0}; // sum of path

  int ans{0};
  long targetSum{0};

public:
  int pathSum(TreeNode* root, int targetSum) {
    if (!root) {
      return 0;
    }

    this->targetSum = targetSum;

    dfs(root);
    return ans;
  }

  void dfs(TreeNode* n) {
    // check all paths ending at n
    if (n->val == targetSum) {
      ans++;
    }
    long prefix{0};
    for (auto& pn : path) {
      if (sum + n->val - prefix == targetSum) {
	ans++;
      }
      prefix += pn->val;
    }

    path.push_back(n);
    sum += n->val;

    if (n->left) {
      dfs(n->left);
    }
    if (n->right) {
      dfs(n->right);
    }

    // backtracking
    path.pop_back();
    sum -= n->val;
  }
};
