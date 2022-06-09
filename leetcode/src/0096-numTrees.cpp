#include <vector>
#include <cmath>

using namespace std;

class Solution {
public:
  int numTrees(int n) {
    vector<int> f(n + 1);
    f[0] = 1;

    for (int i = 1; i <= n; i++) {
      int k = std::floor(i / 2);

      for (int j = 0; j < k; j++) {
	f[i] += f[j] * f[i - 1 - j];
      }
      f[i] *= 2;

      if (i % 2) {
	f[i] += f[k];
      }
    }

    return f[n];
  }
};
