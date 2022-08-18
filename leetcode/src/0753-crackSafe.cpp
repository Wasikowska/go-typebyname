#include <cmath>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Solution {
public:
  string crackSafe(int n, int k) {
    stringstream ans;
    if (n == 1) {
      for (int i = 0; i < k; i++) {
	ans << char(i + '0');
      }
      return ans.str();
    }

    // we use a bit mask to indicate edges
    int nodes = pow(k, n - 1);
    vector<int> g(nodes, pow(2, k) - 1);

    vector<int> st;
    st.emplace_back(0);
    while (!st.empty()) {
      int v = st.back();

      // find next edge
      int to{-1};
      for (int i = 0; i < k; i++) {
	if (g[v] & (1 << i)) {
	  to = (v * k + i) % nodes;
	  break;
	}
      }

      if (to == -1) {
	// v has 0 degree
	st.pop_back();
	if (!st.empty()) {
	  // the two ends of st are the same
	  ans << char(v % k + '0');
	}
      } else {
	// delete (v, to)
	int i = to % k;
	g[v] &= ~(1 << i);
	st.push_back(to);
      }
    }

    for (int i = 0; i < n - 1; i++) {
      ans << '0';
    }
    return ans.str();
  }
};
