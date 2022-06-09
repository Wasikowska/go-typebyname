#include <vector>

using namespace std;

class Solution {
public:
  int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
    int row = obstacleGrid.size();
    if (row <= 0) {
      return 0;
    }

    int col = obstacleGrid[0].size();
    if (col <= 0) {
      return 0;
    }

    if (obstacleGrid[0][0] || obstacleGrid[row - 1][col - 1]) {
      return 0;
    }

    vector<vector<int>> f(row, vector<int>(col));
    f[0][0] = 1;
    
    for (int i = 1; i < row; i++) {
      f[i][0] = (obstacleGrid[i][0] ^ 1) & f[i - 1][0];
    }
    for (int j = 1; j < col; j++) {
      f[0][j] = (obstacleGrid[0][j] ^ 1) & f[0][j - 1];
    }

    for (int i = 1; i < row; i++) {
      for (int j = 1; j < col; j++) {
	if (obstacleGrid[i][j]) {
	  f[i][j] = 0;
	} else {
	  f[i][j] = f[i - 1][j] + f[i][j - 1];
	}
      }
    }

    return f[row - 1][col - 1];
  }
};
