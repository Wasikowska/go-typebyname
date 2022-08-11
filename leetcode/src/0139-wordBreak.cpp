#include <string>
#include <vector>

using namespace std;

class Solution {
private:
  static const int k = 26;
  struct Vertex {
    int next[k];
    bool leaf{false};
    Vertex() {
      fill(begin(next), end(next), -1);
    }
  };

  vector<Vertex> trie{1};

  void addWord(const string& w) {
    int v{0};
    for (int i = 0; i < w.size(); i++) {
      int c = w[i] - 'a';
      if (trie[v].next[c] == -1) {
	trie[v].next[c] = trie.size();
	trie.emplace_back();
      }
      v = trie[v].next[c];
    }
    trie[v].leaf = true;
  }

  enum MemorizedResult {
    UNKNOWN,
    PASS,
    FAIL
  };

  bool solve(const string& s, int idx, vector<MemorizedResult>& r) {
    if (idx >= s.size() || r[idx] == PASS) {
      return true;
    } else if (r[idx] == FAIL) {
      return false;
    }

    int v{0};
    for (int i = idx; i < s.size(); i++) {
      int c = s[i] - 'a';
      v = trie[v].next[c];
      if (v == -1) {
	return r[idx] = FAIL, false;
      } else if (trie[v].leaf && solve(s, i + 1, r)) {
	return r[idx] = PASS, true;
      }
    }

    return r[idx] = FAIL, false;
  }

public:
  bool wordBreak(string s, vector<string>& wordDict) {
    for (auto& word : wordDict) {
      addWord(word);
    }

    vector<MemorizedResult> r(s.size());
    return solve(s, 0, r);
  }
};
