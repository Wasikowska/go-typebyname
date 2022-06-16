#include <vector>
#include <string>
#include <unordered_set>
#include <map>

using namespace std;

// bit Pattern: XX_BIT VERTICAL_BIT HORIZONTAL_BIT
enum Shape { DR = 0b00, DL = 0b01, UR = 0b10, UL = 0b11, XX = 0b100 };

// move direction
using Dir = pair<int, int>;

// if we approach char from Dir, what shape do we get?
map<pair<char, Dir>, Shape> shape = {
    {{'/', {1, 0}}, UL},  {{'/', {-1, 0}}, DR},
    {{'/', {0, 1}}, UL},  {{'/', {0, -1}}, DR},

    {{'\\', {1, 0}}, DL}, {{'\\', {-1, 0}}, UR},
    {{'\\', {0, 1}}, UR}, {{'\\', {0, -1}}, DL},

    {{' ', {1, 0}}, XX},  {{' ', {-1, 0}}, XX},
    {{' ', {0, 1}}, XX},  {{' ', {0, -1}}, XX},
};

// move tables
Dir dirXX[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

Dir dir[4][2] = {
    // DR
    {{0, 1}, {1, 0}},

    // DL
    {{0, 1}, {-1, 0}},

    // UR
    {{0, -1}, {1, 0}},

    // UL
    {{0, -1}, {-1, 0}},
};

struct Node {
  int x;
  int y;
  Shape s;

  bool operator==(const Node& n) const {
    return x == n.x && y == n.y && s == n.s;
  }
};

namespace std {
  template <>
  struct hash<Node> {
    size_t operator()(const Node& n) const {
      return hash<int>()(n.x) ^ hash<int>()(n.y) ^ hash<int>()(n.s);
    }
  };
  } // namespace std

class Solution {
private:
  //unordered_set<Node> visited;
  // replacing hashset with array can improve performance a lot
  bool visited[30][30][5];

  optional<Node> findStart(vector<string>& grid) {
    int m = grid.size();
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < m; j++) {
	char c = grid[i][j];
	if (c == ' ') {
	  // one XX node
	  if (!visited[i][j][XX]) {
	    return Node{i, j, XX};
	  }
	} else if (c == '/') {
	  // two nodes, UL annd DR
	  if (!visited[i][j][UL]) {
	    return Node{i, j, UL};
	  } else if (!visited[i][j][DR]) {
	    return Node{i, j, DR};
	  }
	} else if (c == '\\') {
	  // two nodes, UR annd DL
	  if (!visited[i][j][UR]) {
	    return Node{i, j, UR};
	  } else if (!visited[i][j][DL]) {
	    return Node{i, j, DL};
	  }	  
	}
      }
    }
    return nullopt;
  }

public:
  int regionsBySlashes(vector<string>& grid) {
    int m = grid.size();
    int ans{0};

    for (int i = 0; i < 30; i++) {
      for (int j = 0; j < 30; j++) {
	for (int k = 0; k < 5; k++) {
	  visited[i][j][k] = false;
	}
      }
    }

    while (auto start = findStart(grid)) {
      vector<Node> stack;
      stack.push_back(*start);

      while (!stack.empty()) {
	Node n = stack.back();
	stack.pop_back();

	if (visited[n.x][n.y][n.s]) {
	  continue;
	}
	visited[n.x][n.y][n.s] = true;

	auto [x, y, s] = n;

	if (s == XX) {
	  for (int i = 0; i < 4; i++) {
	    int nx = x + dirXX[i].first;
	    int ny = y + dirXX[i].second;

	    if (nx >= 0 && nx < m
		&& ny >= 0 && ny < m) {
	      Node nn = {nx, ny, shape[{grid[nx][ny], dirXX[i]}]};
	      stack.push_back(nn);
	    }
	  }
	} else {
	  for (int i = 0; i < 2; i++) {
	    int nx = x + dir[s][i].first;
	    int ny = y + dir[s][i].second;

	    if (nx >= 0 && nx < m
		&& ny >= 0 && ny < m) {
	      Node nn = {nx, ny, shape[{grid[nx][ny], dir[s][i]}]};

	      if (nn.s == XX) {
		stack.push_back(nn);
	      } else if (dir[s][i].first == 0) {
		// move vertically
		if ((s ^ nn.s) & 0b10) {
		  stack.push_back(nn);
		}
	      } else {
		// move horizontally
		if ((s ^ nn.s) & 1) {
		  stack.push_back(nn);
		}
	      }
	    }	    
	  }
	}
      }

      ans++;
    }

    return ans;
  }
};
