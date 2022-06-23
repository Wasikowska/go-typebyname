#include <vector>
#include <map>

using namespace std;

class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
      int n = nums.size();
      vector<int> f(n, 1);
      multimap<int, int> m;

      int ans{0};
      for (int i = 0; i < n; i++) {
	auto end = m.lower_bound(nums[i]);
	for (auto j = m.begin(); j != end; j++) {
	  f[i] = max(f[i], j->second + 1);
	}
	m.insert({nums[i], f[i]});
	ans = max(ans, f[i]);
      }

      return ans;
    }
};
