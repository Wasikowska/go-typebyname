#include <string>
#include <unordered_set>

using namespace std;

class Solution {
private:
  int p1 = 29;
  int m1 = 1e9 + 9;

  int p2 = 37;
  int m2 = 1e9 + 7;

public:
  string longestPrefix(string s) {
    unordered_set<int64_t> hs1, hs2;

    int64_t h1{0}, h2{0};
    int64_t pe1{1}, pe2{1};
    for (int i = 0; i < s.size(); i++) {
      h1 = (h1 + (s[i] - 'a' + 1) * pe1) % m1;
      hs1.insert(h1);
      h2 = (h2 + (s[i] - 'a' + 1) * pe2) % m2;
      hs2.insert(h2);
      pe1 = pe1 * p1 % m1;
      pe2 = pe2 * p2 % m2;
    }

    h1 = 0, h2 = 0;
    int idx = s.size();
    for (int i = s.size() - 1; i > 0; i--) {
      h1 = (h1 * p1 % m1 + (s[i] - 'a' + 1)) % m1;
      h2 = (h2 * p2 % m2 + (s[i] - 'a' + 1)) % m2;
      if (hs1.count(h1) && hs2.count(h2)) {
	idx = i;
      }
    }

    return idx == s.size() ? "" : s.substr(idx);
  }
};
