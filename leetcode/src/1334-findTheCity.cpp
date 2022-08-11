#include <vector>

using namespace std;

class Solution {
  const int INF = 1e9;

public:
  int findTheCity(int n, vector<vector<int>>& edges,
		  int distanceThreshold) {
    // use Floyd-Warshall to calculate all-pairs shortest path
    vector<vector<int>> sp(n, vector<int>(n, INF));
    for (int i = 0; i < n; i ++) {
      sp[i][i] = 0;
    }
    for (auto& e : edges) {
      int u = e[0], v = e[1], w = e[2];
      if (w > distanceThreshold) {
	w = INF;
      }
      sp[u][v] = sp[v][u] = w;
    }

    for (int k = 0; k < n; k++) {
      for (int i = 0; i < n; i++) {
	for (int j = 0; j < n; j++) {
	  if (sp[i][k] != INF && sp[k][j] != INF &&
	      sp[i][k] + sp[k][j] < sp[i][j]) {
	    sp[i][j] = sp[i][k] + sp[k][j];
	    if (sp[i][j] > distanceThreshold) {
	      sp[i][j] = INF;
	    }
	  }
	}
      }
    }

    int ans{-1}, minCnt{n + 1};
    for (int i = 0; i < n; i++) {
      int cnt{0};
      for (int j = 0; j < n; j++) {
	if (sp[i][j] < INF) {
	  cnt++;
	}
      }
      if (cnt <= minCnt) {
	minCnt = cnt;
	ans = i;
      }
    }

    return ans;
  }
};
