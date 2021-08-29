#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Solution {
private:
  double arr[3];
  int exprCase{0};
  vector<int> cards;

  double calc(char op, double d1, double d2) {
    switch (op) {
    case '+':
      return d1 + d2;
    case '-':
      return d1 - d2;
    case '*':
      return d1 * d2;
    case '/':
      return d1 / d2;
    default:
      return 0;
    }
  }

  bool useOp(char op, int i) {
    if (exprCase == 0) {
      switch (i) {
      case 0:
	if (op == '/' && cards[1] == 0) {
	  return false;
	}
	arr[i] = calc(op, cards[0], cards[1]);
	break;
      case 1:
	if (op == '/' && cards[2] == 0) {
	  return false;
	}
	arr[i] = calc(op, arr[0], cards[2]);
	break;
      case 2:
      default:
	if (op == '/' && cards[3] == 0) {
	  return false;
	}
	arr[i] = calc(op, arr[1], cards[3]);
	break;
      }
    } else if (exprCase == 1) {
      switch (i) {
      case 0:
	if (op == '/' && cards[1] == 0) {
	  return false;
	}
	arr[i] = calc(op, cards[0], cards[1]);
	break;
      case 1:
	if (op == '/' && cards[3] == 0) {
	  return false;
	}
	arr[i] = calc(op, cards[2], cards[3]);
	break;
      case 2:
      default:
	if (op == '/' && arr[1] == 0) {
	  return false;
	}
	arr[i] = calc(op, arr[0], arr[1]);
	break;
      }
    } else if (exprCase == 2) {
      switch (i) {
      case 0:
	if (op == '/' && cards[2] == 0) {
	  return false;
	}
	arr[i] = calc(op, cards[1], cards[2]);
	break;
      case 1:
	if (op == '/' && arr[0] == 0) {
	  return false;
	}
	arr[i] = calc(op, cards[0], arr[0]);
	break;
      case 2:
      default:
	if (op == '/' && cards[3] == 0) {
	  return false;
	}
	arr[i] = calc(op, arr[1], cards[3]);
	break;
      }
    } else if (exprCase == 3) {
      switch (i) {
      case 0:
	if (op == '/' && cards[2] == 0) {
	  return false;
	}
	arr[i] = calc(op, cards[1], cards[2]);
	break;
      case 1:
	if (op == '/' && cards[3] == 0) {
	  return false;
	}
	arr[i] = calc(op, arr[0], cards[3]);
	break;
      case 2:
      default:
	if (op == '/' && arr[1] == 0) {
	  return false;
	}
	arr[i] = calc(op, cards[0], arr[1]);
	break;
      }
    } else if (exprCase == 4) {
      switch (i) {
      case 0:
	if (op == '/' && cards[3] == 0) {
	  return false;
	}
	arr[i] = calc(op, cards[2], cards[3]);
	break;
      case 1:
	if (op == '/' && arr[0] == 0) {
	  return false;
	}
	arr[i] = calc(op, cards[1], arr[0]);
	break;
      case 2:
      default:
	if (op == '/' && arr[1] == 0) {
	  return false;
	}
	arr[i] = calc(op, cards[0], arr[1]);
	break;
      }
    }
    return true;
  }

  bool solve(int i) {
    if (i == 3) {
      return abs(arr[2] - 24.000) < 0.001;
    }

    string ops = "+-*/";
    for (char op : ops) {
      if (!useOp(op, i)) {
	continue;
      }
      if (solve(i + 1)) {
	return true;
      }
    }

    return false;
  }

public:
  bool judgePoint24(vector<int>& cards) {
    this->cards.resize(4);
    for (int i = 0; i < 5; i++) {
      exprCase = i;
      vector<int> index = {0, 1, 2, 3};
      do {
	for (int j = 0; j < 4; j++) {
	  this->cards[j] = cards[index[j]];
	}
	if (solve(0)) {
	  return true;
	}
      }
      while (std::next_permutation(index.begin(), index.end()));
    }
    return false;
  }
};

// int main() {
//   Solution s;
//   vector<int> cards = {3, 3, 8, 8};
//   cout << s.judgePoint24(cards) << endl;
// }
