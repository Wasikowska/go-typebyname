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
class node : public binary_tree::node_base<node, binary_tree::HeightMixin> {
public:
  node(int v) : binary_tree::node_base<node, binary_tree::HeightMixin>(v) {}
};

class tree : public binary_tree::tree_base<tree, node, binary_tree::BSTMixin> {
public:
  bool validate();

  void add(int i);
  void remove(int i);

private:
  void fix_path(std::deque<node *> &path);
};

// check balance factor for each node
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
  if (!root->validate_height())
    return false;

  _balance_fail = false;
  _balance(root);
  return !_balance_fail;
}

void tree::add(int v) {
  std::deque<node *> path;
  bool added = bst_add(root, v, path);

  if (added) {
    // TODO: when we add an element, we only need to fix the first
    // unbalanced node, searching from bottom to up
    fix_path(path);
  }
}

void tree::remove(int v) {
  std::deque<node *> path;
  bool removed = bst_remove(root, v, path);

  if (removed) {
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
    if (node::get_height(n->left) - node::get_height(n->right) > 1) {
      // it is VERY IMPORTANT that we use '>=' instead of '>' here
      if (node::get_height(n->left->left) >= node::get_height(n->left->right)) {
        // left-left rotation
        auto a = n;
        auto b = n->left;
        auto c = n->left->left;

        a->left = b->right;
        b->right = a;

        // the update order is important!
        node::update_height(a);
        node::update_height(b);

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

        node::update_height(a);
        node::update_height(b);
        node::update_height(c);

        new_n = c;
      }
    } else if (node::get_height(n->right) - node::get_height(n->left) > 1) {
      if (node::get_height(n->right->right) >=
          node::get_height(n->right->left)) {
        // right-right rotation
        auto a = n;
        auto b = n->right;
        auto c = n->right->right;

        a->right = b->left;
        b->left = a;

        node::update_height(a);
        node::update_height(b);

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

        node::update_height(a);
        node::update_height(b);
        node::update_height(c);

        new_n = c;
      }
    } else {
      node::update_height(n);
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
  for (int i = 0; i < 500; i++) {
    t.add(rand() % 500);
    ASSERT_TRUE(t.validate());
  }
}

TEST(avl, remove) {
  tree t;
  srand(getpid());
  std::vector<int> addvec, remvec;
  for (int i = 0; i < 500; i++) {
    int v = rand() % 500;
    addvec.push_back(v);
    t.add(v);
  }
  for (int i = 0; i < 400; i++) {
    int v = rand() % 500;
    remvec.push_back(v);
    t.remove(v);
    bool valid = t.validate();
    if (!valid) {
      std::ofstream fs("avl_remove_fail.inc");
      fs << "TEST(avl, fail_remove) {" << std::endl;
      fs << "  tree t;" << std::endl;
      for (auto a : addvec) {
        fs << "  t.add(" << a << ");" << std::endl;
      }
      for (auto r : remvec) {
        fs << "  t.remove(" << r << ");" << std::endl;
      }
      fs << "}" << std::endl;
      fs.close();
    }
    ASSERT_TRUE(valid);
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
