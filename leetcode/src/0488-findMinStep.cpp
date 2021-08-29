#include <algorithm>
#include <string>
#include <vector>
#include <bitset>
#include <map>
#include <iostream>
#include <variant>

using namespace std;

class Solution {
private:
  // state info
  std::string hand;
  int ballUsed{0};
  // ---------

  int min{-1};
  void erase(const string& board, char c, int& start, int& end, bool cExtra) {
    int s{start}, e{end};
    int len = e - s;
    while (s > 0 && board[s - 1] == c) {
      s--;
    }
    while (e < board.size() && board[e] == c) {
      e++;
    }

    if (e - s - len >= (cExtra ? 2 : 3)) {
      start = s;
      end = e;

      if (s > 0 && e < board.size() &&
	  board[s - 1] == board[e]) {
	erase(board, board[s - 1], start, end, false);
      }
    }
  }
  
  string put(const string& board, int pos, char c) {
    int ds{pos};
    int de{pos};
    erase(board, c, ds, de, true);
    if (ds == de) {
      // must keep c
      return board.substr(0, ds) + c + board.substr(de);
    } else {
      // c is erased
      return board.substr(0, ds) + board.substr(de);
    }
  }

  void updateMin() {
    if (min == -1) {
      min = ballUsed;
    } else if (ballUsed < min) {
      min = ballUsed;
    }    
  }

  void solve(const string& board) {
    for (int i = 0; i < hand.size(); i++) {
      char c = hand[i];
      if (c == ' ') {
	continue;
      }

      hand[i] = ' ';
      ballUsed++;
      int j{0};
      while (j <= board.size()) {
	string newboard = put(board, j, c);
	if (newboard.empty()) {
	  updateMin();
	} else if (ballUsed < hand.size()) {
	  if (min == -1 || ballUsed < min ) {
	    bool validHand = any_of(hand.begin(), hand.end(), [&newboard](char c) {
	      return c != ' ' && newboard.find(c) != string::npos;
	    });
	    if (validHand) {
	      solve(newboard);
	    }
	  }
	}
	while (j < board.size() && c == board[j]) {
	  j++;
	}
	j++;
      }
      hand[i] = c;
      ballUsed--;
    }
  }

public:
  int findMinStep(string board, string hand) {
    this->hand = hand;
    this->ballUsed = 0;
    this->min = -1;

    solve(board);
    return min;
  }
};

int main() {
  Solution s;
  // cout << s.findMinStep("RBYYBBRRB", "YRBGB") << endl; // 3
  // cout << s.findMinStep("G", "GGGG") << endl; // 2
  // cout << s.findMinStep("WWRRBBWW", "WRBRW") << endl; // 2
  // cout << s.findMinStep("WRRBBW", "RB") << endl; // -1
  // cout << s.findMinStep("RWYWRRWRR", "YRY") << endl; // 3
  // cout << s.findMinStep("RRWWRRBBRR", "WB") << endl; // 2
  cout << s.findMinStep("RRYGGYYRRYYGGYRR", "BGGBB") << endl; // 5
  // cout << s.findMinStep("RRYGGYYRBRYYGGYRR", "GGBB") << endl; // 4
}
