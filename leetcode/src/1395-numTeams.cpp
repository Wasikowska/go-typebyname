#include <algorithm>
#include <vector>

using namespace std;

class Solution {
private:
  vector<int> bitl, bitr;

  void add(vector<int>& bits, int idx, int val) {
    while (idx < bits.size()) {
      bits[idx] += val;
      idx += idx & -idx;
    }
  }

  int sum(vector<int>& bits, int idx) {
    int s{0};
    while (idx > 0) {
      s += bits[idx];
      idx -= idx & -idx;
    }
    return s;
  }
  
public:
  int numTeams(vector<int>& rating) {
    int n = rating.size();

    vector<int> rating2 = rating;
    rating2.push_back(-1);
    sort(rating2.begin(), rating2.end());
    auto id = [&rating2](int v) -> int {
      return lower_bound(rating2.begin(), rating2.end(), v) - rating2.begin();
    };

    bitl.resize(1001, 0);
    bitr.resize(1001, 0);
    for (int i = 1; i < rating2.size(); i++) {
      bitr[i]++;
      int p = i + (i & -i);
      if (p < rating2.size()) {
	bitr[p] += bitr[i];
      }
    }
    add(bitr, id(rating[0]), -1);

    int ans{0};
    for (int i = 0; i < n - 1; i++) {
      int idx = id(rating[i]);
      int ls = sum(bitl, idx - 1);
      int lb = i - ls;
      int rs = sum(bitr, idx - 1);
      int rb = n - i - 1 - rs;

      ans += ls * rb + lb * rs;

      add(bitl, idx, 1);
      add(bitr, id(rating[i + 1]), -1);
    }

    return ans;
  }
};
