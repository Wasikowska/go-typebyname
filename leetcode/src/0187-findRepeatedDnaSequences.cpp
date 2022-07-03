#include <vector>
#include <string>
#include <unordered_map>
#include <cstdio>

using namespace std;

class Solution {
public:
  int p = 5;
  int m = 1e9 + 9;

  int inv(int a) {
    // format's little theorem
    // a ^ (m - 1) = 1 mod m
    // so inv(a) = a ^ (m - 2)
    // calculate a ^ (m - 2) using binary exponention
    int64_t ans{1};
    int64_t aa = a;
    for (int x = m - 2; x > 0; x >>= 1, aa = (aa * aa) % m) {
      if (x & 1) {
	ans = (ans * aa) % m;
      }
    }
    return ans;
  }

public:
  vector<string> findRepeatedDnaSequences(string s) {
    int n = s.size();
    if (n < 10) {
      return {};
    }

    vector<int64_t> pe(n);
    pe[0] = 1;
    for (int i = 1; i < n; i++) {
      pe[i] = (pe[i - 1] * p) % m;
    }

    vector<int64_t> prefix_hash(n + 1);
    prefix_hash[0] = 0;
    for (int i = 0; i < n; i++) {
      prefix_hash[i + 1] = (prefix_hash[i] + (s[i] - 'A' + 1) * pe[i]) % m;
    }

    // mapping each hash value to starting index
    unordered_map<int64_t, vector<int>> hs;
    for (int i = 0; i + 10 <= n; i++) {
      int64_t h = ((prefix_hash[i + 10] - prefix_hash[i] + m) * inv(pe[i])) % m;
      hs[h].push_back(i);
    }

    vector<string> ans;
    for (auto& [_, v] : hs) {
      if (v.size() > 1) {
	ans.push_back(s.substr(v[0], 10));
      }
    }

    return ans;
  }
};

// int main() {
//   Solution s;
  
//   string str = "AAAAAAAAAAA";
//   auto ans = s.findRepeatedDnaSequences(str);
//   for (auto ss : ans) {
//     printf("%s\n", ss.c_str());
//   }
// }
