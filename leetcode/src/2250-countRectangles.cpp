#include <algorithm>
#include <vector>

using namespace std;

class Solution {
private:
  vector<int> bits;

  void incr(int idx) {
    while (idx < bits.size()) {
      bits[idx]++;
      idx += idx & -idx;
    }
  }

  int sum(int idx) {
    int s{0};
    while (idx > 0) {
      s += bits[idx];
      idx -= idx & -idx;
    }
    return s;
  }
  
public:
  vector<int> countRectangles(vector<vector<int>>& rectangles, vector<vector<int>>& points) {
    bits.resize(101, 0);
    for (int i = 0; i < points.size(); i++) {
      // append index
      points[i].push_back(i);
    }

    auto cmp = [](const vector<int>& r1, const vector<int>& r2) {
      // sort by x index, in descending order
      return r1[0] > r2[0];
    };

    sort(rectangles.begin(), rectangles.end(), cmp);
    sort(points.begin(), points.end(), cmp);

    vector<int> ans(points.size());
    int cnt{0}, ri{0};
    for (int i = 0; i < points.size(); i++) {
      int x = points[i][0];
      int y = points[i][1];
      int idx = points[i][2];

      while (ri < rectangles.size() && rectangles[ri][0] >= x) {
	incr(rectangles[ri][1]);
	ri++;
	cnt++;
      }

      ans[idx] = cnt - sum(y - 1);
    }

    return ans;
  }
};
