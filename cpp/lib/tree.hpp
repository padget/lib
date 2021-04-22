#ifndef __lib_tree_hpp__
#define __lib_tree_hpp__

#include "vector.hpp"
#include "exception.hpp"

namespace lib
{
  BASIC_EXCEPTION(tree_exception)
  INHERITED_EXCEPTION(parent_index_doesnt_exist, tree_exception)
  INHERITED_EXCEPTION(previous_index_doesnt_exist, tree_exception)
  
  constexpr std::size_t no_next = static_cast<std::size_t>(-1);
  constexpr std::size_t no_child = static_cast<std::size_t>(-1);
  constexpr std::size_t no_root = static_cast<std::size_t>(-1);

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
    using node_type = tree_node<key_t, value_t>;

    vector<node_type> nodes;

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
    inline std::size_t push_root(
      const key_t &key, const value_t &val)
    {
      if (nodes.empty())
        nodes.emplace_back(node_type{key, val});

      return 0;
    }

    inline std::size_t push_next(
        const key_t &key,
        const value_t &value,
        std::size_t index)
    {
      if (index == no_root)
        throw previous_index_doesnt_exist();
      
      if (index > nodes.back_index())
        throw previous_index_doesnt_exist();

      nodes.emplace_back(node_type{key, value});

      node_type &newnode = nodes.back();
      std::size_t newnext = nodes.back_index();

      node_type &prev = nodes[index];
      std::size_t oldnext = prev.next;

      prev.next = newnext;
      newnode.next = oldnext;

      return newnext;
    }

    inline std::size_t push_child(
        const key_t &key,
        const value_t &value,
        std::size_t index)
    {
      if (index == no_root)
        throw parent_index_doesnt_exist();
      
      if (index > nodes.back_index())
        throw parent_index_doesnt_exist();

      nodes.emplace_back(node_type{key, value});

      node_type &newnode = nodes.back();
      std::size_t newchild = nodes.back_index();

      node_type &parent = nodes[index];
      std::size_t oldchild = parent.child;

      parent.child = newchild;
      newnode.next = oldchild;

      return newchild;
    }

    inline const node_type &operator[](std::size_t i) const { return nodes[i]; }
    inline node_type &operator[](std::size_t i) { return nodes[i]; }

    inline tree_childs<key_t, value_t> childs_of(std::size_t index = 0)
    {
      return {nodes[index].child, *this};
    }
  };
}

#endif