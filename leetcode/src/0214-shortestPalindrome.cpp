#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Solution {
public:
  string shortestPalindrome(string s) {
    // the point is to find the longest prefix which is also a palindrome,
    // in order to do that, we calculate the prefix array for s + reverse(s)
    int n = s.size();
    vector<int> pi(n + n);

    int j{0}; // j = pi[i - 1]
    for (int i = 1; i < n; i++) {
      while (j > 0 && s[i] != s[j]) {
	j = pi[j - 1];
      }
      if (s[i] == s[j]) {
	j++;
      }
      pi[i] = j;
    }

    for (int i = 0; i < n; i++) {
      int ii = n - 1 - i;
      while (j > 0 && s[j] != s[ii]) {
	j = pi[j - 1];
      }
      if (s[j] == s[ii]) {
	j++;
      }
      pi[i + n] = j;
    }

    string ans = s.substr(j);
    reverse(ans.begin(), ans.end());
    ans += s;

    return ans;
  }
};
