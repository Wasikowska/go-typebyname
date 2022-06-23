#include <vector>

using namespace std;

class Solution {
private:
  vector<int> parents; // disjoint set
  vector<int> sizes;

  int find(int i) {
    if (i != parents[i]) {
      parents[i] = find(parents[i]);
    }
    return parents[i];
  }

  void merge(int i, int j) {
    int r1 = find(i);
    int r2 = find(j);
    if (r1 != r2) {
      if (sizes[r1] < sizes[r2]) {
	swap(r1, r2);
      }
      parents[r2] = r1;
      sizes[r1] += sizes[r2];
    }
  }

public:
    bool hasValidPath(vector<vector<int>>& grid) {
      int row = grid.size();
      int col = grid[0].size();

      parents.resize(row * col);
      sizes.resize(row * col, 1);
      for (int i = 0; i < parents.size(); i++) {
	parents[i] = i;
      }

      for (int i = 0; i < row; i++) {
	for (int j = 0; j < col; j++) {
	  int idx = i * col + j;
	  int v = grid[i][j];
	  if (j < col - 1) {
	    int v2 = grid[i][j + 1];
	    if ((v == 1 || v == 4 || v == 6)
		&& (v2 == 1 || v2 == 3 || v2 == 5)) {
	      merge(idx, idx + 1);
	    }
	  }
	  if (i < row - 1) {
	    int v2 = grid[i + 1][j];
	    if ((v == 2 || v == 3 || v == 4)
		&& (v2 == 2 || v2 == 5 || v2 == 6)) {
	      merge(idx, idx + col);
	    }
	  }
	}
      }

      return find(0) == find(row * col - 1);
    }
};
