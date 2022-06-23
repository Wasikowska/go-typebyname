#include <vector>
#include <deque>

using namespace std;

class Solution {
public:
    int longestSubarray(vector<int>& nums, int limit) {
      if (limit < 0) {
	return 0;
      }

      int s{0}, e{0};
      int ans{1};
      deque<int> maxQueue{0}, minQueue{0};
      int n = nums.size();
      while (e < n - 1) {
	if (s > e) {
	  // mq is empty
	  e++;
	  maxQueue.push_back(e);
	  minQueue.push_back(e);
	  continue;
	}

	int maxValue = nums[maxQueue.front()];
	int minValue = nums[minQueue.front()];
	int nv = nums[e + 1];
	if (nv <= minValue + limit && nv >= maxValue - limit) {
	  e++;
	  while (!maxQueue.empty() && nums[maxQueue.back()] < nv) {
	    maxQueue.pop_back();
	  }
	  while (!minQueue.empty() && nums[minQueue.back()] > nv) {
	    minQueue.pop_back();
	  }
	  maxQueue.push_back(e);
	  minQueue.push_back(e);
	} else {
	  // the sliding window is about to shrink
	  ans = max(ans, e - s + 1);
	  if (maxQueue.front() == s) {
	    maxQueue.pop_front();
	  }
	  if (minQueue.front() == s) {
	    minQueue.pop_front();
	  }
	  s++;
	}
      }
      return max(ans, e - s + 1);
    }
};
