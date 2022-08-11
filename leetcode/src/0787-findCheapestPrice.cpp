#include <vector>

using namespace std;

class Solution {
  const int INF = 1e9;

public:
  int findCheapestPrice(int n, vector<vector<int>>& flights,
			int src, int dst, int k) {
    vector<int> sp(n, INF);
    sp[src] = 0;

    // bellman-ford algorithm
    for (int i = 0; i < k + 1; i++) {
      bool relax{false};
      auto clone = sp;
      for (auto& e : flights) {
	int u = e[0], v = e[1], w = e[2];
	if (clone[u] != INF && clone[u] + w < sp[v]) {
	  sp[v] = clone[u] + w;
	  relax = true;
	}
      }
      if (!relax) {
	break;
      }
    }

    return sp[dst] == INF ? -1 : sp[dst];
  }
};
