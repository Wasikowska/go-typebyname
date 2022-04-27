#include <string>

using namespace std;

using Size = string::size_type;
using Range = pair<Size, Size>;

bool cmpRange(const Range& r1, const Range& r2) {
  return (r1.second - r1.first) < (r2.second - r2.first);
}

class Solution {
public:
    string longestPalindrome(string s) {
      if (s.length() < 2) {
	return s;
      }

      Range max = {0, 0};

      // expand(s, L - 1, L - 1) must be 1, so it's safe to ignore it
      for (Size i = 0; i < s.length() - 1; i ++) {
	max = std::max(max, expand(s, i, i), cmpRange);
	if (s[i] == s[i + 1]) {
	  max = std::max(max, expand(s, i, i + 1), cmpRange);
	}
      }

      return s.substr(max.first, max.second - max.first + 1);
    }

private:
  // expand s[i, j]
  Range expand(const string& s, Size i, Size j) {
    while (i > 0 && j < s.length() - 1 && s[i - 1] == s[j + 1]) {
      i--;
      j++;
    }
    return {i, j};
  }
};
