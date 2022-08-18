#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

class Solution {
public:
  vector<string> findItinerary(vector<vector<string>>& tickets) {
    // adjacency list representation
    map<string, multiset<string>> g;
    for (auto& e : tickets) {
      string& from = e[0], to = e[1];
      g[from].emplace(to);
    }

    vector<string> ans;

    // "JFK" is guaranteed to be a start point
    vector<string> st;
    st.emplace_back("JFK");
    while (!st.empty()) {
      string v = st.back();
      if (g[v].empty()) {
	ans.push_back(v);
	st.pop_back();
      } else {
	// choose the smallest node to continue;
	const string& to = *(g[v].begin());
	st.push_back(to);
	g[v].erase(g[v].begin());
      }
    }

    reverse(ans.begin(), ans.end());
    return ans;
  }
};
