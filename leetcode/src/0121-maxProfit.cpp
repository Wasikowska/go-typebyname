#include <vector>

using namespace std;

class Solution {
public:
  int maxProfit(vector<int>& prices) {
    if (prices.empty()) {
      return 0;
    }
    int low = prices[0];
    int gain = 0;
    for (int i = 1; i < prices.size(); i++) {
      low = std::min(low, prices[i]);
      gain = std::max(gain, prices[i] - low);
    }
    return gain;
  }
};
