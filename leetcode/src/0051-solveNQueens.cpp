#include <string>
#include <vector>
#include <bitset>
#include <iostream>
#include <algorithm>

using namespace std;

class Solution {
private:
  int size{0};
  std::vector<std::bitset<2>> check;
  std::vector<std::bitset<2>> checkCross;
  vector<string> board;
  vector<vector<string>> results;

  void solve(int col) {
    if (col >= size) {
      results.push_back(board);
      return;
    }

    // find a position for the queen
    for (int row = 0; row < size; row++)  {
      if (!check[row][0] && !check[col][1] &&
	  !checkCross[row + col][0] &&
	  !checkCross[row + size - 1 - col][1]) {
	board[row][col] = 'Q';
	check[row].set(0);
	check[col].set(1);
	checkCross[row + col].set(0);
	checkCross[row + size - 1 - col].set(1);

	solve(col + 1);

	board[row][col] = '.';
	check[row].reset(0);
	check[col].reset(1);
	checkCross[row + col].reset(0);
	checkCross[row + size - 1 - col].reset(1);
      }
    }
  }
public:
    vector<vector<string>> solveNQueens(int n) {
      size = n;
      check.resize(n);
      checkCross.resize(n + n - 1);
      board.resize(n, string(n, '.'));
      solve(0);
      return results;
    }
};

// int main() {
//   auto r = Solution().solveNQueens(4);
//   for_each(r.begin(), r.end(), [](auto& v) {
//     cout << "------------------------" << std::endl;
//     for_each(v.begin(), v.end(), [](auto& str) {
//       std::cout << str << std::endl;
//     });
//   });
// }
