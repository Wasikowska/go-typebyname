#include <cmath>
#include <vector>

using namespace std;

class Solution {
  const int INF = 1e9;

public:
  int minCostConnectPoints(vector<vector<int>>& points) {
    int n = points.size();
    vector<bool> marked(n);

    vector<int> min_e(n, INF);
    min_e[0] = 0;

    int ans{0};
    for (int i = 0; i < n; i++) {
      int v = -1;
      for (int j = 0; j < n; j++) {
	if (!marked[j] && (v == -1 || min_e[j] < min_e[v])) {
	  v = j;
	}
      }

      marked[v] = true;
      ans += min_e[v];

      for (int j = 0; j < n; j++) {
	int dist = abs(points[v][0] - points[j][0]) + abs(points[v][1] - points[j][1]);
	if (dist < min_e[j]) {
	  min_e[j] = dist;
	}
      }
    }

    return ans;
  }
};
