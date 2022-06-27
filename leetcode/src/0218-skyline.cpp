#include <algorithm>
#include <vector>
#include <set>

using namespace std;

enum PointKind {
  UP,
  DOWN,
};

struct Point {
  PointKind kind;
  int x{0};
  int height{0};

  bool operator<(const Point& p) {
    return x < p.x;
  }
};

class Solution {
public:
  vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
    vector<Point> ps(buildings.size());
    for (auto& b : buildings) {
      ps.push_back({UP, b[0], b[2]});
      ps.push_back({DOWN, b[1], b[2]});
    }
    sort(ps.begin(), ps.end());

    multiset<int> hs{0};
    vector<vector<int>> ans;

    auto i = ps.begin();
    while (i < ps.end()) {
      auto x = i->x;
      int top = *hs.rbegin();
      do {
	auto [k, _, h] = *i;
	if (k == UP) {
	  hs.insert(h);
	} else if (k == DOWN) {
	  hs.erase(hs.find(h));
	}
      } while (++i < ps.end() && x == i->x);

      if (*hs.rbegin() != top) {
	ans.push_back({x, *hs.rbegin()});
      }
    }

    return ans;
  }
};
