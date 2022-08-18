#include <algorithm>
#include <vector>

using namespace std;

class Solution {
  class DisjointSet {
    vector<int> p;
    vector<int> size;
    int sets{0};

  public:
    DisjointSet(int n) {
      sets = n;
      p.resize(n);
      for (int i = 0; i < n; i++) {
	p[i] = i;
      }
      size.resize(n, 1);
    }

    int find(int v) {
      if (v != p[v]) {
	p[v] = find(p[v]);
      }
      return p[v];
    }

    bool merge(int u, int v) {
      int ru = find(u), rv = find(v);
      if (ru != rv) {
	if (size[ru] < size[rv]) {
	  swap(u, v);
	}
	p[v] = u;
	size[u] += size[v];
	sets--;
	return true;
      }
      return false;
    }

    int getSets() {
      return sets;
    }
  };

public:
  vector<vector<int>> findCriticalAndPseudoCriticalEdges(int n, vector<vector<int>>& edges) {
    // save index
    for (int i = 0; i < edges.size(); i++) {
      edges[i].push_back(i);
    }

    sort(edges.begin(), edges.end(), [](const vector<int>& e1, const vector<int>& e2) {
      return e1[2] < e2[2];
    });

    vector<vector<int>> ans(2); // ans[1]: critical, ans[2]: pseduo critical

    int mst{0};
    DisjointSet ds(n);
    for (auto& e : edges) {
      if (ds.merge(e[0], e[1])) {
	mst += e[2];
      }
    }

    for (auto& e : edges) {
      DisjointSet ds2(n);
      int mst2{0};

      // check if e is a critical edge
      for (auto& e2 : edges) {
	if (e == e2) {
	  continue;
	}
	if (ds2.merge(e2[0], e2[1])) {
	  mst2 += e2[2];
	}
      }
      if (ds2.getSets() > 1 || mst2 > mst) {
	ans[0].push_back(e[3]);
	continue;
      }

      // e is not critical, is it pseduo critical?
      ds2 = DisjointSet(n);
      mst2 = 0;
      ds2.merge(e[0], e[1]);
      mst2 += e[2];

      for (auto& e2 : edges) {
	if (ds2.merge(e2[0], e2[1])) {
	  mst2 += e2[2];
	}
      }

      if (mst2 == mst) {
	ans[1].push_back(e[3]);
      }
    }

    return ans;
  }
};
