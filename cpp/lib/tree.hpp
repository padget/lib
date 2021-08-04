#ifndef __lib_tree_hpp__
#define __lib_tree_hpp__

#include <lib/exception.hpp>
#include <lib/vector.hpp>
#include <lib/assert.hpp>

namespace lib
{
  BASIC_EXCEPTION(tree_exception)
  INHERITED_EXCEPTION(parent_index_doesnt_exist, tree_exception)
  INHERITED_EXCEPTION(previous_index_doesnt_exist, tree_exception)

  constexpr unsigned no_next = static_cast<unsigned>(-1);
  constexpr unsigned no_child = static_cast<unsigned>(-1);
  constexpr unsigned no_root = static_cast<unsigned>(-1);

  template <
      typename value_t>
  struct tree_node;

  template <
      typename value_t>
  class tree;

  template <
      typename value_t>
  struct child_iterator
  {
    tree<value_t> *t = nullptr;
    long unsigned index;

    inline tree_node<value_t> &
    operator*()
    {
      return (*t)[index];
    }

    inline const tree_node<value_t> &
    operator*() const
    {
      return (*t)[index];
    }

    inline child_iterator &
    operator++()
    {
      if (index != no_next)
        index = (*t)[index].next;

      return *this;
    }

    inline child_iterator
    operator++(int)
    {
      child_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    inline bool
    operator==(
        const child_iterator o) const
    {
      return index == o.index;
    }

    inline bool
    operator!=(
        const child_iterator o) const
    {
      return not operator==(o);
    }
  };

  template <
      typename value_t>
  struct tree_childs
  {
    using iterator = child_iterator<value_t>;
    using const_iterator = child_iterator<value_t>;

    size_t first;
    tree<value_t> *t = nullptr;

    iterator
    begin() { return {t, first}; }

    iterator
    end() { return {t, no_next}; }

    const_iterator
    begin() const { return {t, first}; }

    const_iterator
    end() const { return {t, no_next}; }
  };

  template <
      typename value_t>
  struct tree_node
  {
    value_t value;
    tree<value_t> *tr = nullptr;
    size_t child = no_child;
    size_t next = no_next;
    size_t last_child = no_child;

    bool has_child() const
    {
      return child != no_child;
    }

    bool has_next() const
    {
      return next != no_next;
    }

    tree_childs<value_t>
    childs() { return {child, tr}; }

    const tree_childs<value_t>
    childs() const { return {child, tr}; }
  };

  template <
      typename value_t>
  class tree
  {
    vector<tree_node<value_t>> nodes;

  public:
    tree(size_t _max = 10)
        : nodes(_max) {}

  public:
    inline size_t size() const
    {
      return nodes.size();
    }

  public:
    inline size_t
    push_root(const value_t &val)
    {
      if (nodes.empty())
        nodes.push_back({val, this});

      return 0;
    }

    inline size_t
    push_back_child(
        value_t &&value,
        size_t parent_id)
    {
      contract(parent_id != no_root, parent_index_doesnt_exist());
      contract(parent_id <= nodes.back_index(), previous_index_doesnt_exist());
      
      auto &parent = nodes[parent_id];
      auto nnid = nodes.size();
      auto &id = parent.child == no_child ?
                 parent.child :
                 nodes[parent.last_child].next;
      id = nnid;
      parent.last_child = nnid;
      nodes.push_back({move(value), this}); 

      return nnid;
    }

    inline size_t
    push_back_child(
        const value_t &value,
        size_t parent_id)
    {
      return push_back_child(
          move(value_t(value)), parent_id);
    }

    inline const tree_node<value_t> &
    operator[](size_t i) const
    {
      return nodes[i];
    }

    inline tree_node<value_t> &
    operator[](size_t i)
    {
      return nodes[i];
    }

    inline tree_childs<value_t>
    childs(size_t index = 0)
    {
      return {nodes[index].child, this};
    }

    inline const tree_childs<value_t>
    childs(size_t index = 0) const
    {
      return {nodes[index].child, this};
    }
  };
}

#endif
