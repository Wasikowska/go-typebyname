#include "tree.h"

// 旋转的时候，需要
namespace splay_tree {
struct node : public binary_tree::node_base<node> {
  node(int v) : binary_tree::node_base<node>(v) {}
};

class tree : public binary_tree::tree_base<tree, node, binary_tree::BSTMixin> {
public:
  void add(int v);
  void remove(int v);

private:
  void splay(std::deque<node *> &path);
};

void tree::add(int v) {
  std::deque<node *> path;
  bool added = bst_add(root, v, path);

  if (added) {
    splay(path);
  }
}

// this is top-down splay tree; there's also bottom up splay tree
void tree::remove(int v) {
  std::deque<node *> path;
  bool removed = bst_remove(root, v, path);

  if (removed) {
    splay(path);
  }
}

void tree::splay(std::deque<node *> &path) {
  node *a{nullptr}, *b{nullptr}, *c{nullptr};

  for (;;) {
    if (path.empty())
      break;
    if (path.size() == 1) {
      root = path.back();
      break;
    }

    c = path.back();
    path.pop_back();

    b = path.back();
    path.pop_back();

    if (path.empty()) {
      // b is the root, this is the final rotation
      if (b->value > c->value) {
        b->left = c->right;
        c->right = b;
      } else {
        b->right = c->left;
        c->left = b;
      }
    } else {
      a = path.back();
      path.pop_back();

      if (b->value < a->value && c->value < b->value) {
        // zig-zig rotation
        b->left = c->right;
        a->left = b->right;
        b->right = a;
        c->right = b;
      } else if (b->value > a->value && c->value > b->value) {
        // zag-zag rotation
        b->right = c->left;
        a->right = b->left;
        b->left = a;
        c->left = b;
      } else if (b->value < a->value && c->value > b->value) {
        // zig-zag rotation
        b->right = c->left;
        a->left = c->right;
        c->left = b;
        c->right = a;
      } else {
        // zag-zig rotation
        b->left = c->right;
        a->right = c->left;
        c->right = b;
        c->left = a;
      }
    }

    path.push_back(c);
  }
}

TEST(splay_tree, add) {
  tree t;
  t.add(15);
  t.print(test_info_, "1");
  t.add(10);
  t.print(test_info_, "2");
  t.add(17);
  t.print(test_info_, "3");
  t.add(7);
  t.print(test_info_, "4");
  t.add(13);
  t.print(test_info_, "5");
  t.add(16);
  t.print(test_info_, "6");
  t.add(14);
  t.print(test_info_, "7");
  t.add(12);
  t.print(test_info_, "8");
}

TEST(splay_tree, remove) {
  tree t;
  t.add_list({15, 10, 17, 7, 13, 16, 14, 12});

  // bst remove is not ready yet
  t.remove(16);
  t.print(test_info_, "1");
  t.remove(12);
  t.print(test_info_, "2");
  t.remove(7);
  t.print(test_info_, "3");
}

} // namespace splay_tree
