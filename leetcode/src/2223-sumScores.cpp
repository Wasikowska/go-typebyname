#include <string>
#include <vector>

using namespace std;

class Solution {
public:
  long long sumScores(string s) {
    // create z function for s
    int n = s.size();
    vector<int> z(n);

    long long ans{n};

    // [l, r) is the rightmost calculated z-suffix
    for (int i = 1, l = 0, r = 1; i < n; i++) {
      if (i < r) {
	z[i] = min(z[i - l], r - i);
      }
      while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
	z[i]++;
      }
      if (z[i] > r) {
	r = i + z[i], l = i;
      }
      ans += z[i];
    }

    return ans;
  }
};
