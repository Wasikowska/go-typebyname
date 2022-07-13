#include <string>
#include <vector>

using namespace std;

class Solution {
public:
  bool repeatedSubstringPattern(string s) {
    int n = s.size();
    vector pi(n, 0);

    int j = pi[0];
    for (auto i = 1; i < n; i++) {
      while (j > 0 && s[j] != s[i]) {
	j = pi[j - 1];
      }
      if (s[j] == s[i]) {
	j++;
      }
      pi[i] = j;
    }

    return j != 0 && !(n % (n - j));
  }
};
