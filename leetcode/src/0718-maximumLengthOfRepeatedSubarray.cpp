#include <vector>
#include <cstdio>

using namespace std;

class Solution {
public:
    int findLength(vector<int>& nums1, vector<int>& nums2) {
      int row = nums1.size(), col = nums2.size();
      int ans = 0;

      vector<vector<int>> dp(row, vector<int>(col));
      for (int i = 0; i < row; i++) {
	dp[i][col - 1] = (nums1[i] == nums2[col - 1]) ? 1 : 0;
	ans = max(ans, dp[i][col - 1]);
	// printf("dp[%d][%d] = %d\n", i, col - 1, dp[i][col - 1]);
      }
      for (int i = 0; i < col; i++) {
	dp[row - 1][i] = (nums1[row - 1] == nums2[i]) ? 1 : 0;
	ans = max(ans, dp[row - 1][i]);
	// printf("dp[%d][%d] = %d\n", row - 1, i, dp[row - 1][i]);
      }

      for (int i = row - 2; i >= 0; i--) {
	for (int j = col - 2; j >= 0; j--) {
	  if (nums1[i] == nums2[j]) {
	    dp[i][j] = dp[i + 1][j + 1] + 1;
	  } else {
	    dp[i][j] = 0;
	  }
	  ans = max(ans, dp[i][j]);
	  // printf("dp[%d][%d] = %d\n", i, j, dp[i][j]);
	}
      }

      return ans;
    }
};

// int main() {
//   vector<int> v1 = {1, 2, 3, 2, 1};
//   vector<int> v2 = {3, 2, 1, 4, 7};

//   Solution s;
//   printf("ans = %d\n", s.findLength(v1, v2));
// }
