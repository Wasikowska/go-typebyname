#include <vector>
#include <set>
#include <utility>
#include <deque>

using namespace std;

class Solution {
private:
  vector<vector<bool>> mark;

public:
  void solve(vector<vector<char>>& board) {
    int row = board.size();
    int col = board[0].size();

    mark.resize(row, vector<bool>(col));

    for (int i = 0; i < row; i++) {
      if (board[i][0] == 'O') {
	dfs(board, i, 0);
      }
      if (board[i][col - 1] == 'O') {
	dfs(board, i, col - 1);
      }
    }
    for (int i = 0; i < col; i++) {
      if (board[0][i] == 'O') {
	dfs(board, 0, i);
      }
      if (board[row - 1][i] == 'O') {
	dfs(board, row - 1, i);
      }
    }

    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
	if (board[i][j] == 'O' && !mark[i][j]) {
	  board[i][j] = 'X';
	}
      }
    }
  }

  void dfs(vector<vector<char>>& board, int i, int j) {
    if (mark[i][j]) {
      return;
    }

    int row = board.size();
    int col = board[0].size();

    using Point = std::pair<int, int>;
    int dir[4][2] = {
      {0, 1},
      {0, -1},
      {1, 0},
      {-1, 0},
    };

    std::set<Point> visited;
    std::deque<Point> stack;
    stack.push_back({i, j});

    while (!stack.empty()) {
      Point p = stack.back();
      stack.pop_back();

      if (visited.count(p)) {
	continue;
      }

      mark[p.first][p.second] = true;
      visited.insert(p);

      for (int i = 0; i < 4; i++) {
	Point np = {p.first + dir[i][0], p.second + dir[i][1]};
	if (np.first >= 0 && np.first < row
	    && np.second >= 0 && np.second < col
	    && board[np.first][np.second] == 'O') {
	  stack.push_back(np);
	}
      }
    }
  }
};
