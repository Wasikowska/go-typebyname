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

template <typename NodeType> class tree {
public:
  virtual void add(int i) = 0;
  virtual void remove(int i) = 0;

  void add_list(const std::initializer_list<int> &list) {
    for (int v : list)
      add(v);
  }

  void remove_list(const std::initializer_list<int> &list) {
    for (auto i : list)
      remove(i);
  }

  void print(testing::TestInfo *test, const std::string &extra = "") {
    std::stringstream tag;
    tag << test->test_case_name() << "_" << test->name();

    std::stringstream ss;
    ss << "digraph D {" << std::endl;
    ss << "bgcolor=grey" << std::endl;

    // bfs traversal
    std::queue<NodeType *> q;
    if (root)
      q.push(root);

    int null_index{1};
    while (!q.empty()) {
      NodeType *n = q.front();
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

  operator NodeType *() { return root; }

protected:
  NodeType *root{nullptr};
};

} // namespace binary_tree
