#include <string>
#include <iostream>
#include <unordered_set>

using namespace std;

// two pointer approach
class Solution {
public:
  int lengthOfLongestSubstring(string s) {
    // the range is [n1, n2)
    int n1{0}, n2{0};
    // better to use an array instead
    std::unordered_set<char> count;
    int max{0};
    while (n2 < s.size()) {
      if (count.find(s[n2]) == count.end()) {
	count.insert(s[n2++]);
      } else {
	if (max < n2 - n1) {
	  max = n2 - n1;
	}
	while (s[n1] != s[n2]) {
	  count.erase(s[n1++]);
	}
	n1++;
	n2++;
      }
    }
    if (max < n2 - n1) {
      max = n2 - n1;
    }
    return max;
  }
};

// int main() {
//   std::cout << Solution().lengthOfLongestSubstring("a") << std::endl;
// }
