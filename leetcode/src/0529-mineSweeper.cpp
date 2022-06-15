#include <vector>
#include <deque>

using namespace std;

using Point = pair<int, int>;

class Solution {
private:
  deque<Point> store;

public:
    vector<vector<char>> updateBoard(vector<vector<char>>& board, vector<int>& click) {
      int x = click[0];
      int y = click[1];

      int row = board.size();
      int col = board[0].size();

      char& c = board[x][y];
      if (c == 'M') {
	c = 'X';
      } else if (c == 'E') {
	deque<Point> queue;
	queue.push_back({x, y});
	board[x][y] = 'B';

	int dir[8][2] = {
	  {-1, -1}, {0, -1}, {1, -1},
	  {-1, 0}, {1, 0},
	  {-1, 1}, {0, 1}, {1, 1},	  
	};

	while (!queue.empty()) {
	  auto [px, py] = queue.front();
	  queue.pop_front();

	  int numOfMines{0};
	  store.resize(0);
	  for (int i = 0; i < 8; i++) {
	    Point np = {px + dir[i][0], py + dir[i][1]};
	    auto [npx, npy] = np;

	    if (npx >= 0 && npx < row
		&& npy >= 0 && npy < col) {
	      if (board[npx][npy] == 'M') {
		numOfMines++;
	      } else if (board[npx][npy] == 'E') {
		store.push_back(np);
	      }
	    }
	  }

	  if (numOfMines) {
	    board[px][py] = '0' + numOfMines;
	  } else {
	    // recursive update
	    // here we set the char to 'B'
	    // so that we can avoid the inqueue test in bfs
	    for (auto& p : store) {
	      board[p.first][p.second] = 'B';
	      queue.push_back(p);
	    }
	  }
	}
      }
      return board;
    }
};
