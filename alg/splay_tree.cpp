#include "tree.h"

// 旋转的时候，需要
namespace splay_tree {
struct node : public binary_tree::node_base<node> {
  node(int v) : binary_tree::node_base<node>(v) {}
};

class tree : public binary_tree::tree<node> {
public:
  void add(int v);
  void remove(int v);

private:
  void splay(std::deque<node *> &path);
};

void tree::add(int v) {
  node *n = root;
  std::deque<node *> path;
  while (true) {
    if (!n) {
      // insert v;
      auto nn = new node(v);
      if (path.empty()) {
        root = nn;
      } else if (path.back()->value > v) {
        path.back()->left = nn;
      } else {
        path.back()->right = nn;
      }
      path.push_back(nn);
      splay(path);
      break;
    } else if (n->value == v) {
      break;
    } else if (n->value > v) {
      path.push_back(n);
      n = n->left;
    } else {
      path.push_back(n);
      n = n->right;
    }
  }
}

// this is top-down splay tree; there's also bottom up splay tree
void tree::remove(int v) {
  node *n = root;
  std::deque<node *> path;
  for (;;) {
    if (!n) {
      // can not find the node to delete
      return;
    }

    path.push_back(n);
    if (n->value == v) {
      // go on to find the alternative
      node *remainder{nullptr};
      if (n->left) {
        node *m = n->left;
        while (m) {
          path.push_back(m);
          m = m->right;
        }
        remainder = path.back()->left;
      } else if (n->right) {
        node *m = n->right;
        while (m) {
          path.push_back(m);
          m = m->left;
        }
        remainder = path.back()->right;
      }

      if (n->left || n->right) {
        // the end of the path is the alternative, we should delete
        // alternative and put its value into node n
        auto alternative = path.back();
        path.pop_back();

        // delete alternate node
        if (remainder) {
          if (remainder->value < path.back()->value) {
            path.back()->left = remainder;
          } else {
            path.back()->right = remainder;
          }
        } else if (alternative->value < path.back()->value) {
          path.back()->left = nullptr;
        } else {
          path.back()->right = nullptr;
        }

        n->value = alternative->value;
      } else {
        // really need to delete n
        assert(n == path.back());
        path.pop_back();

        if (path.empty()) {
          root = nullptr;
        } else if (path.back()->value > n->value) {
          path.back()->left = nullptr;
        } else {
          path.back()->right = nullptr;
        }
      }

      splay(path);
      break;
    } else if (n->value > v) {
      n = n->left;
    } else {
      n = n->right;
    }
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
