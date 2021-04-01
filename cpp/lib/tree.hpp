#ifndef __lib_tree_hpp__
#define __lib_tree_hpp__

#include "vector.hpp"

namespace lib
{
  constexpr std::size_t no_next = static_cast<std::size_t>(-1);
  constexpr std::size_t no_child = static_cast<std::size_t>(-1);
  constexpr std::size_t no_root = static_cast<std::size_t>(-1);

  enum class index_type
  {
    parent,
    pred
  };

  template <typename key_t, typename value_t>
  struct tree_node
  {
    key_t key;
    value_t value;
    std::size_t child = no_child;
    std::size_t next = no_next;
  };

  template <typename key_t, typename value_t>
  struct tree
  {
    vector<tree_node<key_t, value_t>> nodes;

  public:
    tree() = default;
    tree(const tree &) = default;
    tree(tree &&) = default;
    tree &operator=(const tree &) = default;
    tree &operator=(tree &&) = default;

  public:
    tree(std::size_t _max) : nodes(_max) {}

  public:
    size_t size() const { return nodes.size(); }

  public:
    void push(const key_t &key, const value_t &val, std::size_t index, index_type type)
    {
      if (type == index_type::parent and index != no_root)
        nodes[index].child = nodes.size();

      if (type == index_type::pred and index != no_root)
        nodes[index].next = nodes.size();

      nodes.emplace_back(tree_node<key_t, value_t>{key, val});
    }

    const tree_node<key_t, value_t> &operator[](std::size_t i) const { return nodes[i]; }
  };
}

#endif