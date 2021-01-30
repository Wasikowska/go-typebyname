#include "tree.h"
#include <fstream>

// AVL Tree is a self balancing binary search tree
//
// 每个节点的balance factor不超过1：
// balance factor = abs(height(left child) - height(right child))
//
// 基本思路是，在树的结构发生改变时（增加节点，或者删除节点），通过旋
// 转来调整树的结构，保证所有节点的balance factor不超过1
//
// 4种旋转：
// - left left rotation
// - left right rotation
// - right left rotation
// - right right rotation
//
// 这4种旋转，可用来修正balance factor为2的节点。增加节点，或者删除节
// 点，可能会把某个节点的balance factor变为2，但绝不会超过2，所以可以
// 用rotation来修正。旋转还可以保证，旋转后的树，height一定不增加
//
// 对于balance factor不正确的节点，如何选择用哪种ratation？向下看两层，
// 寻找height大的子节点：
// - 左子节点的左子节点有最大的height：用left left rotation
// - 左子节点的右子节点有最大的height：用left right rotation
// - 右子节点的左子节点有最大的height：用right left rotation
// - 右子节点的右子节点有最大的height：用right right rotation
namespace avl_tree {

using binary_search_tree::get_height;
using binary_search_tree::node;
using binary_search_tree::update_height;

class tree : public binary_search_tree::tree {
public:
  bool validate();

  void add(int i);
  void remove(int i);

private:
  void fix_path(std::deque<node *> &path);
};

static bool _balance_fail{false};
static int _balance(node *n) { // return height
  int h{-1};
  if (!n->left && !n->right) {
    h = 0;
  } else if (!n->left) {
    h = _balance(n->right) + 1;
    if (h > 1) {
      _balance_fail = true;
    }
  } else if (!n->right) {
    h = _balance(n->left) + 1;
    if (h >= 2) {
      _balance_fail = true;
    }
  } else {
    auto left = _balance(n->left);
    auto right = _balance(n->right);
    h = std::max(_balance(n->left), _balance(n->right)) + 1;
    if (std::abs(left - right) > 1) {
      _balance_fail = true;
    }
  }
  return h;
}

bool tree::validate() {
  // check height
  if (!validate_height())
    return false;

  _balance_fail = false;
  _balance(root);
  return !_balance_fail;
}

void tree::add(int v) {
  if (!root) {
    root = new node(v);
    return;
  }

  std::deque<node *> path;

  node *n = root;
  for (;;) {
    if (n->value == v) {
      return;
    }

    path.push_back(n);

    if (n->value > v) {
      if (n->left) {
        n = n->left;
      } else {
        n->left = new node(v);
        break;
      }
    } else {
      if (n->right) {
        n = n->right;
      } else {
        n->right = new node(v);
        break;
      }
    }
  }

  fix_path(path);
}

void tree::remove(int v) {
  std::deque<node *> path;

  node *n = root;
  node *hit{nullptr};
  while (n) {
    if (v < n->value) {
      path.push_back(n);
      n = n->left;
    } else if (v > n->value) {
      path.push_back(n);
      n = n->right;
    } else {
      // we find v
      hit = n;

      // altetnative is the node whose value will be put into the hit
      // node. if we can find alternative, we will delete alternative
      // instead of hit
      node *alternative{nullptr};
      if (n->left) {
        path.push_back(n);
        node *m = n->left;
        while (m) {
          if (!m->right) {
            alternative = m;
            break;
          } else {
            path.push_back(m);
            m = m->right;
          }
        }
      }

      if (alternative) {
        // delete alternative
        if (path.back()->value > alternative->value) {
          path.back()->left = nullptr;
        } else {
          path.back()->right = nullptr;
        }
        hit->value = alternative->value;
        break;
      }

      if (n->right) {
        path.push_back(n);
        node *m = n->right;
        while (m) {
          if (!m->left) {
            alternative = m;
            break;
          } else {
            path.push_back(m);
            m = m->left;
          }
        }
      }

      if (alternative) {
        if (path.back()->value > alternative->value) {
          path.back()->left = nullptr;
        } else {
          path.back()->right = nullptr;
        }
        hit->value = alternative->value;
        break;
      }

      // can not find a alternative, so we must actually delete hit
      if (path.empty()) {
        root = nullptr;
      } else {
        if (path.back()->value > hit->value) {
          path.back()->right = nullptr;
        } else {
          path.back()->left = nullptr;
        }
      }

      break;
    }
  }

  if (hit) {
    fix_path(path);
  }
}

// path is a series of nodes that may violate the avl balance factor
// property
void tree::fix_path(std::deque<node *> &path) {
  while (!path.empty()) {
    node *n = path.back();
    path.pop_back();

    node *new_n{nullptr};
    if (get_height(n->left) - get_height(n->right) > 1) {
      if (get_height(n->left->left) > get_height(n->left->right)) {
        // left-left rotation
        auto a = n;
        auto b = n->left;
        auto c = n->left->left;

        a->left = b->right;
        b->right = a;

        // the update order is important!
        update_height(a);
        update_height(b);

        new_n = b;
      } else {
        // left right rotation
        auto a = n;
        auto b = n->left;
        auto c = n->left->right;

        b->right = c->left;
        a->left = c->right;
        c->left = b;
        c->right = a;

        update_height(a);
        update_height(b);
        update_height(c);

        new_n = c;
      }
    } else if (get_height(n->right) - get_height(n->left) > 1) {
      if (get_height(n->right->right) > get_height(n->right->left)) {
        // right-right rotation
        auto a = n;
        auto b = n->right;
        auto c = n->right->right;

        a->right = b->left;
        b->left = a;

        update_height(a);
        update_height(b);

        new_n = b;
      } else {
        // right-left rotation
        auto a = n;
        auto b = n->right;
        auto c = n->right->left;

        b->left = c->right;
        a->right = c->left;
        c->right = b;
        c->left = a;

        update_height(a);
        update_height(b);
        update_height(c);

        new_n = c;
      }
    } else {
      update_height(n);
      continue;
    }

    // update parent link
    if (path.empty()) {
      root = new_n;
    } else {
      auto &parent = path.back();
      if (parent->value < new_n->value) {
        parent->right = new_n;
      } else {
        parent->left = new_n;
      }
    }
  }
}

TEST(avl, add) {
  tree t;
  srand(getpid());
  for (int i = 0; i < 3000; i++) {
    t.add(rand() % 3000);
    ASSERT_TRUE(t.validate());
  }
}

TEST(avl, remove) {
  tree t;
  srand(getpid());
  for (int i = 0; i < 3000; i++) {
    t.add(rand() % 3000);
  }
  for (int i = 0; i < 2500; i++) {
    t.remove(rand() % 3000);
    ASSERT_TRUE(t.validate());
  }
}

void bm_avl_add(benchmark::State &state) {
  tree t;
  for (auto _ : state) {
    for (int i = 0; i < 100; i++) {
      t.add(rand() % 100);
    }
  }
}

BENCHMARK(bm_avl_add);

} // namespace avl_tree
