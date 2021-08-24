#include <iostream>
#include <utility>
#include <vector>
#include <bitset>

using namespace std;

class Solution {
private:
  vector<vector<char>>* pBoard{nullptr};
  std::bitset<3> check[9][9];

  void next(int& row, int& col) {
    if (col == 8) {
      col = 0;
      row++;
    } else {
      col++;
    }
  }

  bool solve(int row, int col) {
    if (row > 8) {
      return true;
    }

    auto& board = *pBoard;
    while (board[row][col] != '.') {
      next(row, col);
      if (row > 8) {
	return true;
      }
    }

    // try to select a valid char for this blank
    for (char c = '1'; c <= '9'; c++) {
      int num = c - '1';
      int grid = row / 3 * 3 + col / 3;
      if (!check[num][row].test(0) && !check[num][col].test(1) && !check[num][grid].test(2)) {
	board[row][col] = c;
	check[num][row].set(0);
	check[num][col].set(1);
	check[num][grid].set(2);

	int nrow{row};
	int ncol{col};
	next(nrow, ncol);

	if (solve(nrow, ncol)) {
	  return true;
	}

	board[row][col] = '.';
	check[num][row].reset(0);
	check[num][col].reset(1);
	check[num][grid].reset(2);
      }
    }
    return false;
  }

public:
  void solveSudoku(vector<vector<char>>& board) {
    this->pBoard = &board;
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
	char c = board[i][j];
	if (c != '.') {
	  int num = c - '1';
	  int grid = i / 3 * 3 + j / 3;
	  check[num][i].set(0);
	  check[num][j].set(1);
	  check[num][grid].set(2);
	}
      }
    }
    solve(0, 0);
  }

  void printBoard(vector<vector<char>>& board) {
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
	std::cout << board[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }
};

// int main() {
//   vector<vector<char>> board = {{'5','3','.','.','7','.','.','.','.'},
// 				{'6','.','.','1','9','5','.','.','.'},
// 				{'.','9','8','.','.','.','.','6','.'},
// 				{'8','.','.','.','6','.','.','.','3'},
// 				{'4','.','.','8','.','3','.','.','1'},
// 				{'7','.','.','.','2','.','.','.','6'},
// 				{'.','6','.','.','.','.','2','8','.'},
// 				{'.','.','.','4','1','9','.','.','5'},
// 				{'.','.','.','.','8','.','.','7','9'}};

//   Solution s;
//   s.solveSudoku(board);
//   s.printBoard(board);
// }
