#include <algorithm>
#include <vector>

using namespace std;

class Solution {
  const int INF = 1e9;

public:
  int networkDelayTime(vector<vector<int>>& times,
		       int n, int k) {
    // adjacentcy matrix representation
    vector<vector<int>> g(n + 1, vector<int>(n + 1, INF));
    for (int i = 0; i < n + 1; i++) {
      g[i][i] = 0;
    }
    for (auto& e : times) {
      int u = e[0], v = e[1], w = e[2];
      g[u][v] = w;
    }

    vector<int> sp(n + 1, INF);
    vector<bool> marked(n + 1);
    sp[k] = 0;

    for (int i = 1; i <= n; i++) {
      // find the unmarked node having shortest path
      int v{-1};
      for (int j = 1; j <= n; j++) {
	if (!marked[j] && (v == -1 || sp[j] < sp[v])) {
	  v = j;
	}
      }
      marked[v] = true;

      for (int j = 1; j <= n; j++) {
	if (g[v][j] != INF && sp[v] + g[v][j] < sp[j]) {
	  sp[j] = sp[v] + g[v][j];
	}
      }
    }

    auto ans = *max_element(sp.begin() + 1, sp.end());
    return ans == INF ? -1 : ans;
  }
};
