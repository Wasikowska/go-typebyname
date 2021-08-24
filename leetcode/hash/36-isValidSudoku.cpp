#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
      // row: num
      // col: row_num/col_num/grid_num
      // first bit: row
      // second bit: col
      // third bit: grid
      std::bitset<3> check[9][9];
      for (int i = 0; i < 9; i++) {
	for (int j = 0; j < 9; j++) {
	  char c = board[i][j];
	  if (c == '.') {
	    continue;
	  }
	  int num = c - '1';
	  int grid = i / 3 * 3 + j / 3;
	  if (check[num][i].test(0) || check[num][j].test(1) || check[num][grid].test(2)) {
	    return false;
	  }
	  check[num][i].set(0);
	  check[num][j].set(1);
	  check[num][grid].set(2);
	}
      }
      return true;
    }
};
