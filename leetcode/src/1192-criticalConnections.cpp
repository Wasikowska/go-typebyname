#include <vector>

using namespace std;

class Solution {
  vector<int> tin, low;
  vector<bool> visited;
  int time{1};
  vector<vector<int>> g;

  vector<vector<int>> ans;

  void dfs(int v, int p) {
    tin[v] = low[v] = time++;
    visited[v] = true;
    for (int to : g[v]) {
      if (to != p && visited[to]) {
	// (v, to) is a back edge
	low[v] = min(low[v], tin[to]);
      } else if (!visited[to]) {
	dfs(to, v);
	low[v] = min(low[v], low[to]);
      }
      if (tin[v] < low[to]) {
	// (v, to) is a bridge
	ans.push_back({v, to});
      }
    }
  }

public:
  vector<vector<int>> criticalConnections(int n,
					  vector<vector<int>>& connections) {
    tin.resize(n);
    low.resize(n);
    visited.resize(n, false);
    g.resize(n);

    for (auto& con : connections) {
      int u = con[0], v = con[1];
      g[u].emplace_back(v);
      g[v].emplace_back(u);
    }

    for (int i = 0; i < n; i++) {
      if (!visited[i]) {
	dfs(i, -1);
      }
    }

    return ans;
  }
};
