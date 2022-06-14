#include <vector>

using namespace std;

class Solution {
public:
    vector<int> findDiagonalOrder(vector<vector<int>>& mat) {
      int row = mat.size();
      int col = mat[0].size();

      vector<int> ans;
      ans.reserve(row * col);

      for (int sum = 0; sum < row + col - 1; sum++) {
	if (sum % 2) { // traverse to row max, col 0
	  // calculate the start row index
	  int r = 0;
	  if (sum - r >= col) {
	    r = sum - col + 1;
	  }

	  for (; r <= sum && r < row; r++) {
	    ans.push_back(mat[r][sum - r]);
	  }
	} else { // traverse to row 0, col max
	  // calculate the start col index
	  int l = 0;
	  if (sum - l >= row) {
	    l = sum - row + 1;
	  }

	  for (; l <= sum && l < col; l++) {
	    ans.push_back(mat[sum - l][l]);
	  }
	}
      }
      
      return ans;
    }
};
