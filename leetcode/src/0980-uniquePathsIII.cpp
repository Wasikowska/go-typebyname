#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>

using namespace std;

class Solution {
private:
  struct Point {
  public:
    int x{-1};
    int y{-1};

    Point() = default;
    Point(int x, int y) : x(x), y(y) {}

    bool operator==(const Point& p) {
      return x == p.x && y == p.y;
    }
  };

  int row{-1};
  int col{-1};
  vector<vector<bool>> check;
  int num{0};
  Point end;

  optional<Point> up(Point p) {
    if (p.x != 0 && !check[p.x - 1][p.y]) {
      return optional<Point>(Point(p.x - 1, p.y));
    } else {
      return optional<Point>();
    }
  }

  optional<Point> down(Point p) {
    if (p.x != row - 1 && !check[p.x + 1][p.y]) {
      return optional<Point>(Point(p.x + 1, p.y));
    } else {
      return optional<Point>();
    }
  }

  optional<Point> left(Point p) {
    if (p.y != 0 && !check[p.x][p.y - 1]) {
      return optional<Point>(Point(p.x, p.y - 1));
    } else {
      return optional<Point>();
    }
  }

  optional<Point> right(Point p) {
    if (p.y != col - 1 && !check[p.x][p.y + 1]) {
      return optional<Point>(Point(p.x, p.y + 1));
    } else {
      return optional<Point>();
    }
  }

  void walk(Point p) {
    check[p.x][p.y] = true;
    if (p == end) {
      // check if all grids are visited
      bool allVisited = std::all_of(check.begin(), check.end(), [](auto& r) {
	return std::all_of(r.begin(), r.end(), [](bool b) {
	  return b;
	});
      });
      if (allVisited) {
	num++;
      }
    } else {
      if (auto mr = up(p)) {
	walk(*mr);
      }
      if (auto mr = down(p)) {
	walk(*mr);
      }
      if (auto mr = left(p)) {
	walk(*mr);
      }
      if (auto mr = right(p)) {
	walk(*mr);
      }
    }
    check[p.x][p.y] = false;
  }

public:
    int uniquePathsIII(vector<vector<int>>& grid) {
      row = grid.size();
      col = grid[0].size();
      check.resize(row);
      for_each(check.begin(), check.end(), [this](auto& r) {
	r.resize(col);
      });

      Point start;
      for (int i = 0; i < row; i++) {
	for (int j = 0; j < col; j++) {
	  int n = grid[i][j];
	  if (n == -1) {
	    check[i][j] = true;
	  }
	  if (n == 1) {
	    start.x = i;
	    start.y = j;
	  } else if (n == 2) {
	    end.x = i;
	    end.y = j;
	  }
	}
      }
      walk(start);
      return num;
    }
};

// int main() {
//   vector<vector<int>> grid = {{1,0,0,0},{0,0,0,0},{0,0,0,2}};
//   std::cout << Solution().uniquePathsIII(grid) << std::endl;
// }
