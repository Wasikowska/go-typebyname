#include "tree.h"

namespace binary_search_tree {

static bool _height_fail{false};
static int _height(node *n) {
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
}

bool tree::validate_height() {
  if (!root)
    return true;

  _height_fail = false;
  _height(root);
  return !_height_fail;
}

} // namespace binary_search_tree
