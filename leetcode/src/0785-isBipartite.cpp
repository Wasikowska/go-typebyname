#include <vector>

using namespace std;

class Solution {
  vector<int> color; // valid color: {0, 1}
  vector<vector<int>>* g{nullptr};

  bool dfs(int v, int p) {
    if (p == -1 || color[p] == 1) {
      color[v] = 0;
    } else {
      color[v] = 1;
    }

    for (int to : (*g)[v]) {
      if (color[to] == color[v]) {
	return false;
      } else if (color[to] == -1 && !dfs(to, v)) {
	return false;
      }
    }

    return true;
  }

public:
  bool isBipartite(vector<vector<int>>& graph) {
    int n = graph.size();
    color.resize(n, -1);
    g = &graph;
    for (int i = 0; i < n; i++) {
      if (color[i] == -1 && !dfs(i, -1)) {
	return false;
      }
    }
    return true;
  }
};
