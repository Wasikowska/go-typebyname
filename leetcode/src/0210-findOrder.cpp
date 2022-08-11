#include <algorithm>
#include <vector>

using namespace std;

class Solution {
  vector<vector<int>> g;
  vector<int> color; // 0: white, 1: grey, 2: black

  vector<int> ans;

  // if detect a cycle, return false
  bool dfs(int v) {
    color[v] = 1;
    for (int to : g[v]) {
      if (color[to] == 1) {
	// back edge
	return false;
      } else if (color[to] == 0 && !dfs(to)) {
	return false;
      }
    }
    color[v] = 2;
    ans.push_back(v);
    return true;
  }

public:
  vector<int> findOrder(int numCourses,
			vector<vector<int>>& prerequisites) {
    color.resize(numCourses);
    g.resize(numCourses);
    for (auto& e : prerequisites) {
      g[e[1]].push_back(e[0]);
    }

    for (int i = 0; i < numCourses; i++) {
      if (!color[i] && !dfs(i)) {
	return {};
      }
    }

    reverse(ans.begin(), ans.end());
    return ans;
  }
};
