#pragma once

#include <initializer_list>
#include <queue>
#include <sstream>
#include <string>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

namespace binary_tree {

// mainly used in other mixins, to get the derived class pointer
template <typename Derived> struct AsDerivedMixin {
  Derived *as_derived() { return static_cast<Derived *>(this); }
};

// add a height member, plus height checker
template <typename Derived>
struct HeightMixin : public AsDerivedMixin<Derived> {
  int height{0};

  // validate if the height is correctly calculated
  bool validate_height() {
    bool _height_fail{false};
    std::function<int(Derived *)> _height;
    _height = [&](Derived *n) -> int {
      if (!n) {
        return -1;
      }

      int h{-1};
      if (!n->left && !n->right) {
        h = 0;
      } else if (!n->left) {
        h = _height(n->right) + 1;
      } else if (!n->right) {
        h = _height(n->left) + 1;
      } else {
        h = std::max(_height(n->left), _height(n->right)) + 1;
      }
      if (h != n->height) {
        _height_fail = true;
      }
      return h;
    };

    _height(this->as_derived());
    return !_height_fail;
  }

  static int get_height(Derived *n) { return n ? n->height : -1; }
  static void update_height(Derived *n) {
    n->height = std::max(get_height(n->left), get_height(n->right)) + 1;
  }
};

template <typename Derived, template <typename> class... Mixin>
struct node_base : public Mixin<Derived>... {
  Derived *left{nullptr};
  Derived *right{nullptr};

  int value;

  node_base(int v) : value{v} {}

  std::string dot_id() {
    std::stringstream ss;
    ss << value;
    return ss.str();
  }

  virtual std::string dot_rep() {
    std::stringstream ss;
    ss << dot_id() << " [label=\"" << dot_id()
       << "\", style=filled, font=monospace, fontsize=15]";
    return ss.str();
  }
};

template <typename Derived, typename NodeT>
struct BSTMixin : public AsDerivedMixin<Derived> {
public:
  bool bst_add(NodeT *&root, int v, std::deque<NodeT *> &path) {
    assert(path.empty());
    NodeT *n = root; // assign root to n
    for (;;) {
      if (!n) {
        // insert v;
        auto nn = new NodeT(v);
        if (!path.empty()) {
          if (path.back()->value > v) {
            path.back()->left = nn;
          } else {
            path.back()->right = nn;
          }
        } else {
          root = nn;
        }
        path.push_back(nn);
        return true;
      } else if (n->value == v) {
        return false;
      } else if (n->value > v) {
        path.push_back(n);
        n = n->left;
      } else {
        path.push_back(n);
        n = n->right;
      }
    }
  }

  bool bst_remove(NodeT *&root, int v, std::deque<NodeT *> &path) {
    NodeT *n = root;
    for (;;) {
      if (!n) {
        // can not find the node to delete
        return false;
      }

      path.push_back(n);

      if (n->value == v) {
        // go on to find the alternative
        NodeT *remainder{nullptr};
        if (n->left) {
          NodeT *m = n->left;
          while (m) {
            path.push_back(m);
            m = m->right;
          }
          remainder = path.back()->left;
        } else if (n->right) {
          NodeT *m = n->right;
          while (m) {
            path.push_back(m);
            m = m->left;
          }
          remainder = path.back()->right;
        }

        // we should delete the alternative node
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

          // put the altetnative node's value into node n
          n->value = alternative->value;
        } else {
          // we really need to delete n
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

        return true;
      } else if (n->value > v) {
        n = n->left;
      } else {
        n = n->right;
      }
    }
  }
};

template <typename Derived, typename NodeT,
          template <typename, typename> class... Mixin>
class tree_base : public Mixin<Derived, NodeT>... {
protected:
  NodeT *root{nullptr};

public:
  virtual void add(int v) = 0;
  virtual void remove(int v) = 0;

  void add_list(const std::initializer_list<int> &list) {
    for (int v : list)
      add(v);
  }

  void remove_list(const std::initializer_list<int> &list) {
    for (auto i : list)
      remove(i);
  }

  operator NodeT *() { return root; }

  void print(testing::TestInfo *test = nullptr, const std::string &extra = "") {
    std::stringstream tag;
    if (test) {
      tag << test->test_case_name() << "_" << test->name();
    }

    std::stringstream ss;
    ss << "digraph D {" << std::endl;
    ss << "bgcolor=grey" << std::endl;

    // bfs traversal
    std::queue<NodeT *> q;
    if (root)
      q.push(root);

    int null_index{1};
    while (!q.empty()) {
      NodeT *n = q.front();
      q.pop();

      ss << n->dot_rep() << std::endl;

      if (n->left) {
        ss << n->dot_id() << "->" << n->left->dot_id() << std::endl;
        q.push(n->left);
      } else {
        ss << "null" << null_index << " [shape=point,color=grey]" << std::endl;
        ss << n->dot_id() << "->null" << null_index++
           << " [color=\"#8c8c8c\", style=dotted, dir=none]" << std::endl;
      }
      if (n->right) {
        ss << n->dot_id() << "->" << n->right->dot_id() << std::endl;
        q.push(n->right);
      } else {
        ss << "null" << null_index << " [shape=point,color=grey]" << std::endl;
        ss << n->dot_id() << "->null" << null_index++
           << " [color=\"#8c8c8c\", style=dotted, dir=none]" << std::endl;
      }
    }

    ss << "}";

    char tmpfile[] = "XXXXXX.dot";
    int fd = mkstemps(tmpfile, 4);
    if (fd == -1) {
      perror("mkstemps");
      exit(1);
    }

    auto str = ss.str();
    write(fd, str.c_str(), str.size());

    std::stringstream cmd;
    cmd << "dot -Tpng:cairo " << tmpfile << " > " << tag.str() << extra
        << ".png";
    system(cmd.str().c_str());

    close(fd);
    unlink(tmpfile);
  }
};

} // namespace binary_tree
