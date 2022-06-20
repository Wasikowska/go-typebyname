#include <vector>
#include <deque>

using namespace std;

class Solution {
public:
  int maxSubarraySumCircular(vector<int>& nums) {
    return maxSubarraySumCircular2(nums);
  }

  int maxSubarraySumCircular1(vector<int>& nums) {
    // case1: subarray does not cross borders
    // kadane's algorithm
    int n = nums.size();

    int f{nums[0]};
    int ans = f;
    for (int i = 1; i < n; i++) {
      f = max(nums[i], nums[i] + f);
      ans = max(f, ans);
    }

    // case2: subarray crosses borders
    //vector<int> rightSum(n);
    int rightSum{nums[n - 1]};
    vector<int> maxRightSum(n);
    maxRightSum[n - 1] = rightSum;
    for (int j = n - 2; j >= 0; j--) {
      rightSum = nums[j] + rightSum;
      maxRightSum[j] = max(rightSum, maxRightSum[j + 1]);
    }

    int leftSum{0};
    for (int i = 0; i < n - 2; i++) {
      leftSum += nums[i];
      ans = max(ans, leftSum + maxRightSum[i + 2]);
    }

    return ans;
  }

  int maxSubarraySumCircular2(vector<int>& nums) {
    int n = nums.size();
    vector<int> prefixSum{n * 2 + 1};
    prefixSum[0] = 0;
    for (int i = 1; i <= n * 2; i++) {
      prefixSum[i] = prefixSum[i - 1] + nums[(i - 1) % n];
    }

    deque<pair<int, int>> mq; // minimum queue
    int mq_pushed{0}, mq_poped{0};

    auto mq_push_back = [&](int index) {
      while (!mq.empty() && nums[mq.back().first] > nums[index]) {
	mq.pop_back();
      }
      mq.push_back({index, mq_pushed++});
    };
    auto mq_pop_front = [&]() {
      if (!mq.empty() && mq.front().second == mq_poped) {
	mq.pop_front();
      }
      mq_poped++;
    };
    auto mq_min_val = [&]() -> auto {
      return nums[mq.front().first];
    };
    auto mq_size = [&]() -> auto {
      return mq_pushed - mq_poped;
    };

    int ans{nums[0]};
    mq_push_back(0);
    for (int j = 1; j <= n * 2; j++) {
      ans = max(ans, prefixSum[j] - mq_min_val());
      mq_push_back(j);
      if (mq_size() > n) {
	mq_pop_front();
      }
    }

    return ans;
  }
};
