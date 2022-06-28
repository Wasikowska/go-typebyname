#include <algorithm>
#include <vector>
#include <cstdint>
#include <set>

#include <cstdio>

using namespace std;

class Solution {
private:
  vector<long> bit;

  void incr(int i) {
    while (i < bit.size()) {
      bit[i]++;
      i += i & -i;
    }
  }

  int sum(int i) {
    int s{0};
    while (i > 0) {
      s += bit[i];
      i -= i & -i;
    }
    return s;
  }

public:
  int countRangeSum(vector<int>& nums, int lower, int upper) {
    vector<int64_t> prefixSum(nums.size() + 1);
    prefixSum[0] = 0;
    for (int i = 0; i < nums.size(); i++) {
      prefixSum[i + 1] = prefixSum[i] + nums[i];
    }

    vector<int64_t> ps2;
    ps2.reserve(prefixSum.size() * 3);
    for (auto n : prefixSum) {
      ps2.push_back(n);
      ps2.push_back((int64_t)n - lower);
      ps2.push_back((int64_t)n - upper);
    }
    sort(ps2.begin(), ps2.end());
    auto getidx = [&ps2](int64_t n) {
      return lower_bound(ps2.begin(), ps2.end(), n) - ps2.begin() + 1;
    };

    bit.resize(ps2.size() + 1);
    incr(getidx(0));

    int ans{0};
    for (int i = 1; i < prefixSum.size(); i++) {
      ans += sum(getidx(prefixSum[i] - lower)) - sum(getidx(prefixSum[i] - upper) - 1);
      incr(getidx(prefixSum[i]));
    }

    return ans;
  }
};

// int main() {
//   Solution s;
//   vector<int> v = {-2, 5, -1};// {0, -1, -2, -3, 0, 2};
//   auto ans = s.countRangeSum(v, -2, 2);// 3, 5);
//   printf("ans = %d\n", ans);
// }
