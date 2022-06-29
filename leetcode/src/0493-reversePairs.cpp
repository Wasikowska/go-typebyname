#include <vector>
#include <algorithm>
#include <cstdint>

using namespace std;

class Solution {
private:
  vector<int> st;

  void _incr(int ti, int tl, int tr, int id) {
    st[ti]++;
    if (tl == tr) {
      return;
    }

    int tm = (tl + tr) / 2;
    if (id > tm) {
      _incr(ti + 2 * (tm - tl + 1), tm + 1, tr, id);
    } else {
      _incr(ti + 1, tl, tm, id);
    }
  }

  void incr(int id) {
    _incr(0, 0, st.size() / 2 - 1, id);
  }

  int _sum(int ti, int tl, int tr, int l, int r) {
    if (l > r) {
      return 0;
    }

    if (tl == l && tr == r) {
      return st[ti];
    } else {
      int tm = (tl + tr) / 2;
      return _sum(ti + 1, tl, tm, l, min(r, tm)) +
	_sum(ti + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r);
    }
  }

  int sum(int l, int r) {
    return _sum(0, 0, st.size() / 2 - 1, l, r);
  }

public:
  int reversePairs(vector<int>& nums) {
    vector<int64_t> nums2;
    nums2.reserve(nums.size() * 2);
    for (auto n : nums) {
      nums2.push_back(n);
      nums2.push_back((long)n * 2);
    }
    sort(nums2.begin(), nums2.end());
    auto id = [&nums2](int64_t n) {
      return lower_bound(nums2.begin(), nums2.end(), n) - nums2.begin();
    };

    st.resize(nums2.size() * 2);
    incr(id(nums[0]));

    int ans{0};
    for (int j = 1; j < nums.size(); j++) {
      ans += j - sum(0, id((long)nums[j] * 2));
      incr(id(nums[j]));
    }

    return ans;
  }
};
