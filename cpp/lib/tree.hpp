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
  class tree;

  template <typename key_t, typename value_t>
  struct child_iterator
  {
    std::size_t index;
    tree<key_t, value_t> &t;

    value_t &operator*()
    {
      return t[index].value;
    }

    const value_t &operator*() const
    {
      return t[index].value;
    }

    child_iterator &operator++()
    {
      if (index != no_next)
        index = t[index].next;

      return *this;
    }

    child_iterator operator++(int)
    {
      child_iterator tmp(*this);
      ++(*this);
      return tmp;
    }
  };

  template <typename key_t, typename value_t>
  inline bool operator!=(
      const child_iterator<key_t, value_t> l,
      const child_iterator<key_t, value_t> r)
  {
    return l.index != r.index;
  }

  template <typename key_t, typename value_t>
  struct tree_childs
  {
    std::size_t index;
    tree<key_t, value_t> &t;

    child_iterator<key_t, value_t> begin()
    {
      return {index, t};
    }

    child_iterator<key_t, value_t> end()
    {
      return {no_next, t};
    }

    const child_iterator<key_t, value_t> begin() const
    {
      return {index, t};
    }

    const child_iterator<key_t, value_t> end() const
    {
      return {no_next, t};
    }
  };

  template <typename key_t, typename value_t>
  class tree
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
    inline size_t size() const { return nodes.size(); }

  public:
    inline std::size_t push(const key_t &key, const value_t &val, std::size_t index, index_type type)
    {
      lib::println("insert of");
      lib::printfln("index : #, next : #, child : #, value : #", index,
                    (*this)[index].next,
                    (*this)[index].child,
                    (*this)[index].value);

      if (type == index_type::parent and index != no_root)
        nodes[index].child = nodes.size();

      else if (type == index_type::pred and index != no_root)
        nodes[index].next = nodes.size();

      lib::printfln("index : #, next : #, child : #, value : #", index,
                    (*this)[index].next,
                    (*this)[index].child,
                    (*this)[index].value);

      lib::println("end insert of");
      nodes.emplace_back(tree_node<key_t, value_t>{key, val});
      return nodes.size() - 1;
    }

    inline const tree_node<key_t, value_t> &operator[](std::size_t i) const { return nodes[i]; }
    inline tree_node<key_t, value_t> &operator[](std::size_t i) { return nodes[i]; }

    inline tree_childs<key_t, value_t> childs_of(std::size_t index = 0)
    {
      return {nodes[index].child, *this};
    }
  };

}

#endif