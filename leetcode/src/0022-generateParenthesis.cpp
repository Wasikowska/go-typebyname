#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

// this is NOT the right solution !!!
class Solution {
public:
  Solution() {
    parenSet[1] = {"()"};
    parenSet[2] = {"(()), ()()"};
    parenSet[3] = {"()()()", "()(())", "(())()", "(()())", "((()))"};
  }
  
  vector<string> generateParenthesis(int n) {
    if (n <= 3) {
      return parenSet[n];
    }
    for (int i = 4; i <= n; i++) {
      unordered_set<string> unique;
      vector<string>& s = parenSet[i - 1];

      // there ways to construct the new set
      // 1. prepend one parenthesis to the left
      // 2. append one parenthesis to the right
      // 3. wrap one parenthesis in the outside
      for (auto& str : s) {
	unique.insert("()" + str);
	unique.insert(str + "()");
	unique.insert("(" + str + ")");
      }

      vector<string> result;
      for (auto& str : unique) {
	result.emplace_back(str);
      }

      unique.clear();
      parenSet[i] = result;
    }

    return parenSet[n];
  }

private:
  vector<string> parenSet[9];
};
