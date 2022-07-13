#include <string>
#include <vector>

using namespace std;

class Solution {
public:
  int repeatedStringMatch(string a, string b) {
    int an = a.size(), bn = b.size();
    vector pi(bn, 0);

    int j{pi[0]};
    for (int i = 1; i < bn; i++) {
      while (j > 0 && b[i] != b[j]) {
	j = pi[j - 1];
      }
      if (b[i] == b[j]) {
	j++;
      }
      pi[i] = j;
    }

    // append '#' in the end of b
    j = 0;
    for (int i = 0, dup = 1; dup <= bn / an + 2;
	 dup += i == an - 1 ? 1 : 0, i = (i + 1) % an) {
      while (j > 0 && a[i] != b[j]) {
	j = pi[j - 1];
      }
      if (a[i] == b[j]) {
	j++;
      }
      if (j == bn) {
	return dup;
      }
    }

    return -1;
  }
};
