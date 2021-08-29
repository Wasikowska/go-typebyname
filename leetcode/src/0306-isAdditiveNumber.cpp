#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Solution {
private:
  int size{0};
  string num;

  // state
  vector<int64_t> arr;
  int64_t val{-1};

  bool solve(int i) {
    if (i == size) {
      bool valid = (arr.size() > 1 && (arr[arr.size() - 1] + arr[arr.size() - 2]) == val);
      // if (valid) {
      // 	for_each(arr.begin(), arr.end(), [](int i) {
      // 	  cout << i << ' ';
      // 	});
      // 	cout << val << endl;
      // }
      return valid;
    }

    // begin a new val if possible
    if (arr.size() < 2 || (val == arr[arr.size() - 1] + arr[arr.size() - 2])) {
      if (val != -1) {
	arr.push_back(val);
      }
      val = num[i] - '0';

      if (solve(i + 1)) {
	return true;
      }

      if (arr.empty()) {
	val = -1;
      } else {
	val = arr.back();
	arr.pop_back();
      }
    }

    // update val if possible
    if (val > 0 && (arr.size() < 2 || (val < arr[arr.size() - 1] + arr[arr.size() - 2])) && val < 0x7FFFFFFF) {
      int64_t oldval = val;
      val = val * 10 + (num[i] - '0');
      if (solve(i + 1)) {
	return true;
      }
      val = oldval;
    }

    return false;
  }

public:
  bool isAdditiveNumber(string num) {
    this->size = num.size();
    this->num = num;
    this->val = -1;
    this->arr.clear();
    return solve(0);
  }
};

// int main() {
//   Solution s;
//   cout << s.isAdditiveNumber("11235813213455890144") << endl;
//   cout << s.isAdditiveNumber("199100199") << endl;
//   cout << s.isAdditiveNumber("112358") << endl;
//   cout << s.isAdditiveNumber("101") << endl;
// }
