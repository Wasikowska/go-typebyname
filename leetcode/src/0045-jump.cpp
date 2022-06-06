#include <vector>

using namespace std;

using Size = vector<int>::size_type;

class Solution {
public:
    int jump(vector<int>& nums) {
      if (nums.size() <= 1) {
	return 0;
      }

      Size start = 0, end = 0;
      int step = 0;

      while (end >= start) {
	Size newend = end;
	for (Size i = start; i <= end; i++) {
	  if (newend < i + nums[i]) {
	    newend = i + nums[i];
	  }
	}
	step++;
	if (newend >= nums.size() - 1) {
	  return step;
	}
	start = end + 1;
	end = newend;
      }

      return 0;
    }
};
