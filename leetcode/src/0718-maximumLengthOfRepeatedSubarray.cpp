#include <vector>
#include <cstdint>
#include <unordered_set>
#include <cstdio>

using namespace std;

class Solution {
private:
  int p = 103;
  int invp{0};
  int m = 1e9 + 9;

  void calcInvp() {
    int x{m - 2};
    int64_t ans{1};
    int64_t pe{p};
    while (x > 0) {
      if (x & 1) {
	ans = ans * pe % m;
      }
      x >>= 1, pe = pe * pe % m;
    }
    invp = ans;
  }

  bool tryLen(const vector<int>& nums1, const vector<int>& nums2, int len) {
    int64_t h1{0}, h2{0}, pe{1};
    for (int i = 0; i < len; i++) {
      h1 = (h1 + nums1[i] * pe) % m;
      h2 = (h2 + nums2[i] * pe) % m;
      pe = pe * p % m;
    }
    unordered_set<int64_t> hs{h1};
    for (int i = 1; i + len <= nums1.size(); i++) {
      h1 = (h1 - nums1[i - 1] + m + nums1[i + len - 1] * pe) % m * invp % m;
      hs.insert(h1);
    }
    if (hs.count(h2)) {
      return true;
    }
    for (int i = 1; i + len <= nums2.size(); i++) {
      h2 = (h2 - nums2[i - 1] + m + nums2[i + len - 1] * pe) % m * invp % m;
      if (hs.count(h2)) {
	return true;
      }
    }
    return false;
  }

public:
  int findLength(vector<int>& nums1, vector<int>& nums2) {
    calcInvp();
    int l{0}, r = min(nums1.size(), nums2.size()) + 1;
    while (r - l > 1) {
      int mid = (l + r) / 2;
      if (tryLen(nums1, nums2, mid)) {
	l = mid;
      } else {
	r = mid;
      }
    }
    return l;
  }
};

// int main() {
//   vector<int> v1 = {1, 2, 3, 2, 1};
//   vector<int> v2 = {3, 2, 1, 4, 7};

//   Solution s;
//   printf("ans = %d\n", s.findLength(v1, v2));
// }
