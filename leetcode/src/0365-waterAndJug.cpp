#include <unordered_set>
#include <deque>

using namespace std;

using State = pair<int, int>;

namespace std {
  template <>
  struct hash<State> {
    size_t operator()(const State& s) const {
      return hash<int>()(s.first) ^ hash<int>()(s.second);
    }
  };
  } // namespace std

class Solution {
public:
  bool canMeasureWater(int jug1Capacity, int jug2Capacity, int targetCapacity) {
    deque<State> stack;
    stack.push_back({0, 0});
    unordered_set<State> visited;

    while (!stack.empty()) {
      State s = stack.back();
      stack.pop_back();

      if (visited.count(s)) {
	continue;
      }

      visited.insert(s);

      auto [j1, j2] = s;
      if (j1 == targetCapacity
	  || j2 == targetCapacity
	  || j1 + j2 == targetCapacity) {
	return true;
      }

      stack.push_back({0, j2});
      stack.push_back({j1, 0});
      stack.push_back({jug1Capacity, j2});
      stack.push_back({j1, jug2Capacity});

      if (j1 + j2 <= jug2Capacity) {
	stack.push_back({0, j1 + j2});
      } else {
	stack.push_back({j1 + j2 - jug2Capacity, jug2Capacity});
      }

      if (j1 + j2 <= jug1Capacity) {
	stack.push_back({j1 + j2, 0});
      } else {
	stack.push_back({jug1Capacity, j1 + j2 - jug1Capacity});
      }
    }

    return false;
  }
};
