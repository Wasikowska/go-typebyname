#include <vector>
#include <bitset>
#include <deque>
#include <set>
#include <cstdlib>

using namespace std;

class World {
public:
  enum {
    PACIFIC = 0,
    ATLANTIC = 1,
  };

  World(int row, int col) {
    board[0] = vector<vector<bool>>(row, vector<bool>(col));
    board[1] = vector<vector<bool>>(row, vector<bool>(col));
  }

  bool isFlowTo(int i, int j, int pos) {
    return board[pos][i][j];
  }

  void setFlowTo(int i, int j, int pos) {
    board[pos][i][j] = true;
  }

  vector<vector<int>> getResult() {
    vector<vector<int>> result;
    for (int i = 0; i < board[0].size(); i++) {
      for (int j = 0; j < board[0][0].size(); j++) {
	if (board[0][i][j] && board[1][i][j]) {
	  result.push_back({i, j});
	}
      }
    }
    return result;
  }

private:
  vector<vector<bool>> board[2];
};

class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
      row = heights.size();
      col = heights[0].size();

      World world(row, col);
      for (int r = 0; r < row; r++) {
	bfs(r, 0, world, World::PACIFIC, heights);
	bfs(r, col - 1, world, World::ATLANTIC, heights);
      }
      for (int c = 0; c < col; c++) {
	bfs(0, c, world, World::PACIFIC, heights);
	bfs(row - 1, c, world, World::ATLANTIC, heights);
      }

      return world.getResult();
    }

private:
  int row;
  int col;
  
  void bfs(int r, int c, World& w, int pos, const vector<vector<int>>& heights) {
    deque<pair<int, int>> work;
    work.push_back({r, c});

    w.setFlowTo(r, c, pos);

    while (!work.empty()) {
      auto p = work.front();
      work.pop_front();

      int i = p.first;
      int j = p.second;

      // this trick is so COOL
      static int dir[4][2] = {
	{-1, 0},
	{1, 0},
	{0, -1},
	{0, 1},
      };

      for (int d = 0; d < 4; d++) {
	int ni = i + dir[d][0];
	int nj = j + dir[d][1];

	// NOTICE: I used to put a 'inqueue' check here, it's the
	// normal check for a bfs. however, the w.setFlowTo call is
	// invoked after {ni, nj} is put into the queue, the
	// consequence is that the check of w.isFlowTo overshadows the
	// 'inqueue' check
	if (ni >= 0 && ni < row
	    && nj >= 0 && nj < col
	    && heights[ni][nj] >= heights[i][j]
	    && !w.isFlowTo(ni, nj, pos)) {
	  work.push_back({ni, nj});
	  w.setFlowTo(ni, nj, pos);
	}
      }
    }
  }
};

// int main() {
//   Solution s;
//   vector<vector<int>> heights = {{2, 1}, {1, 2}};
//   auto result = s.pacificAtlantic(heights);
//   for (auto& p : result) {
//     printf("[%d, %d]\n", p[0], p[1]);
//   }
//   return 0;
// }
