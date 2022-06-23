#include <vector>
#include <deque>

using namespace std;

class Solution {
public:
  int maxResult(vector<int>& nums, int k) {
    int n = nums.size();
    vector<int> f(n);
    f[0] = nums[0];

    deque<int> mq;
    mq.push_back(0);
    for (int i = 1; i < n; i++) {
      f[i] = f[mq.front()] + nums[i];
      while (!mq.empty() && f[mq.back()] < f[i]) {
	mq.pop_back();
      }
      mq.push_back(i);
      if (i - mq.front() >= k) {
	mq.pop_front();
      }
    }

    return f[n - 1];
  }
};
