#include <algorithm>
#include <vector>

using namespace std;

class Solution {
private:
  vector<int> st;

  void _incr(int ti, int tl, int tr, int idx) {
    if (tl <= idx && idx <= tr) {
      st[ti]++;
      if (tl < tr) {
	int tm = (tl + tr) / 2;
	if (idx > tm) {
	  _incr(ti + 2 * (tm - tl + 1), tm + 1, tr, idx);	  
	} else {
	  _incr(ti + 1, tl, tm, idx);	  
	}
      }
    }
  }
  
  void incr(int idx) {
    _incr(0, 0, st.size() - 1, idx);
  }

  int _sum(int ti, int tl, int tr, int l, int r) {
    if (l > r) {
      return 0;
    }
    if (tl == l && tr == r) {
      return st[ti];
    } else {
      int tm = (tl + tr) >> 1;
      return _sum(ti + 1, tl, tm, l, min(r, tm)) +
	_sum(ti + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r);
    }
  }
  
  int sum(int l, int r) {
    return _sum(0, 0, st.size() - 1, l, r);
  }

public:
  vector<int> countSmaller(vector<int>& nums) {
    vector<int> ans(nums.size());
    ans.back() = 0;

    vector<int> nums2 = nums;
    sort(nums2.begin(), nums2.end());
    auto getidx = [&nums2](int x) {
      return lower_bound(nums2.begin(), nums2.end(), x) - nums2.begin();
    };

    st.resize(nums.size() * 2);
    incr(getidx(nums.back()));
    for (int i = nums.size() - 2; i >= 0; i--) {
      ans[i] = sum(0, getidx(nums[i]) - 1);
      incr(getidx(nums[i]));
    }

    return ans;
  }
};
