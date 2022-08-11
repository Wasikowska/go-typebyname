#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
  double maxProbability(int n, vector<vector<int>>& edges,
			vector<double>& succProb, int start, int end) {
    // adjacentcy list representation for sparse graph
    using Weight = double;
    using Node = int;
    using Edge = pair<Weight, Node>;

    vector<vector<Edge>> graph(n);
    for (int i = 0; i < edges.size(); i++) {
      int u = edges[i][0], v = edges[i][1];
      double w = succProb[i];
      graph[u].emplace_back(w, v);
      graph[v].emplace_back(w, u);
    }

    // dijkstra's algorithm
    vector<double> sp(n, 0);
    sp[start] = 1;

    using ShortestPath = pair<double, Node>;
    priority_queue<ShortestPath> pq;
    pq.emplace(1, start);

    while (!pq.empty()) {
      auto [p, n] = pq.top();
      pq.pop();
      if (sp[n] != p) {
	continue;
      }

      for (auto& [w, to] : graph[n]) {
	if (sp[n] * w > sp[to]) {
	  sp[to] = sp[n] * w;
	  pq.emplace(sp[to], to);
	}
      }
    }

    return sp[end];
  }
};
