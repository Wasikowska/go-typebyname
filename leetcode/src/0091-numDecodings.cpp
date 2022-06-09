#include <string>
#include <vector>

using namespace std;

class Solution {
public:
  int numDecodings(string s) {
    if (s.empty()) {
      return 0;
    }

    int len = s.size();
    vector<int> f(len + 1); // append a trailing zero to eliminate boundry check

    f[len] = 1;
    f[len - 1] = (s[len - 1] != '0');

    for (int i = len - 2; i >=0; i--) {
      if (s[i] == '0') {
	f[i] = 0;
      } else if ((s[i] - '0') * 10 + (s[i + 1] - '0') <= 26) {
	// the condition check used to be "(std::stod(s.substr(i, 2)) <= 26)", which is slow
	f[i] = f[i + 1] + f[i + 2];
      } else {
	f[i] = f[i + 1];
      }
    }

    return f[0];
  }
};
