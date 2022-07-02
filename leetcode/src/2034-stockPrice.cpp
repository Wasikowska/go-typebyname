#include <random>
#include <vector>
#include <cstdio>

using namespace std;

struct Node;
using PNode = Node *;

struct Node {
  int ts{0};
  int price{0};
  int prio;

  int minPrice{0};
  int maxPrice{0};
  PNode left{nullptr};
  PNode right{nullptr};
};

namespace {
  random_device rd;
  mt19937 re(rd());
  uniform_int_distribution<> dist;
} // namespace

PNode createNode(int ts, int price) {
  return new Node{ts, price, dist(re), price, price};
}

class StockPrice {
private:
  PNode treap{nullptr};

  void updateMinMax(PNode t) {
    if (!t) {
      return;
    }

    t->minPrice = t->maxPrice = t->price;
    PNode l = t->left, r = t->right;

    if (l) {
      t->maxPrice = max(t->maxPrice, l->maxPrice);
      t->minPrice = min(t->minPrice, l->minPrice);
    }
    if (r) {
      t->maxPrice = max(t->maxPrice, r->maxPrice);
      t->minPrice = min(t->minPrice, r->minPrice);
    }
  }

  void split(PNode t, PNode& l, PNode& r, int ts) {
    if (!t) {
      l = r = nullptr;
    } else if (t->ts <= ts) {
      split(t->right, t->right, r, ts), l = t;
    } else {
      split(t->left, l, t->left, ts), r = t;
    }
    updateMinMax(l), updateMinMax(r);
  }

  void insert(PNode& t, PNode n) {
    if (!t) {
      t = n;
    }  else if (t->prio < n->prio) {
      split(t, n->left, n->right, n->ts);
      t = n;
    } else {
      insert(n->ts <= t->ts ? t->left : t->right, n);
    }
    updateMinMax(t);
  }

public:
  StockPrice() {}
    
  void update(int timestamp, int price) {
    //printf("               -> update(%d, %d)\n", timestamp, price);
    // try to find timestamp
    vector<PNode> v;
    v.reserve(10);
    auto t = treap;
    while (t) {
      v.push_back(t);
      if (t->ts == timestamp) {
	break;
      } else {
	t = timestamp < t->ts ? t->left : t->right;
      }
    }

    if (t) {
      t->price = price;
      for (auto i = v.rbegin(); i != v.rend(); i++) {
	updateMinMax(*i);
      }
    } else {
      insert(treap, createNode(timestamp, price));
    }
  }
    
  int current() {
    PNode t = treap;
    while (t->right)
      t = t->right;
    //printf("cur = %d\n", t->price);
    return t->price;
  }
    
  int maximum() {
    //printf("max = %d\n", treap->maxPrice);
    return treap->maxPrice;
  }
    
  int minimum() {
    //printf("min = %d\n", treap->minPrice);
    return treap->minPrice;
  }
};

// int main() {
//   StockPrice* sp = new StockPrice();
//   sp->update(38,2308);
//   sp->maximum();
//   sp->current();
//   sp->minimum();
//   sp->maximum();
//   sp->maximum();
//   sp->maximum();
//   sp->minimum();
//   sp->minimum();
//   sp->maximum();
//   sp->update(47,7876);
//   sp->maximum();
//   sp->minimum();
//   sp->update(58,1866);
//   sp->maximum();
//   sp->minimum();
//   sp->current();
//   sp->maximum();
//   sp->update(43,121);
//   sp->minimum();
//   sp->maximum();
//   sp->update(40,5339);
//   sp->maximum();
//   sp->maximum();
//   sp->current();
//   sp->update(32,5339);
//   sp->current();
//   sp->minimum();
//   sp->update(43,6414);
//   sp->update(49,9369);
//   sp->minimum();
//   sp->minimum();
//   sp->update(36,3192);
//   sp->current();
//   sp->update(48,1006);
//   sp->maximum();
//   sp->update(53,8013);
//   sp->minimum(); 
// }

// // 输出：
// // [2308,2308,2308,2308,2308,2308,2308,2308,2308,7876,2308,7876,1866,1866,7876,121,7876,7876,7876,1866,1866,121,121,121,1866,9369,1006]
// // 预期结果：
// // [2308,2308,2308,2308,2308,2308,2308,2308,2308,7876,2308,7876,1866,1866,7876,121,7876,7876,7876,1866,1866,121,1866,1866,1866,9369,1006]
