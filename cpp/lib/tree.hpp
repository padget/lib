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

    size_t first;
    tree<value_t> *t = nullptr;

    iterator
    begin()
    {
      return {t, first};
    }

    iterator
    end()
    {
      return {t, no_next};
    }

    const iterator
    begin() const
    {
      return {t, first};
    }

    const iterator
    end() const
    {
      return {t, no_next};
    }
  };

  template <
      typename value_t>
  struct tree_node
  {
    value_t value;
    tree<value_t> *tr = nullptr;
    size_t id = 0;
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
    childs()
    {
      return {child, tr};
    }

    const tree_childs<value_t>
    childs() const
    {
      return {child, tr};
    }
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
    push_root(
        const value_t &val)
    {
      if (nodes.empty())
        nodes.push_back(tree_node<value_t>{val, this});

      return nodes.back().id;
    }

    inline size_t
    push_back_child(
        const value_t &value,
        size_t parent_id)
    {
      contract(parent_id != no_root, parent_index_doesnt_exist());
      contract(parent_id <= nodes.back_index(), previous_index_doesnt_exist());
      
      tree_node<value_t> &parent = nodes[parent_id];
      nodes.push_back(tree_node<value_t>{value, this});

      tree_node<value_t> &nn = nodes.back();
      size_t nnid = nodes.back_index();
      nn.id = nnid;

      if (parent.child == no_child)
        parent.child = nnid;
      else
      {
        tree_node<value_t> &lcn = nodes[parent.last_child];
        lcn.next = nnid;
      }

      parent.last_child = nnid;

      return nnid;
    }

    inline const tree_node<value_t> &
    operator[](
        size_t i) const
    {
      return nodes[i];
    }

    inline tree_node<value_t> &
    operator[](
        size_t i)
    {
      return nodes[i];
    }

    inline tree_childs<value_t>
    childs(
        size_t index = 0)
    {
      return {nodes[index].child, this};
    }

    inline const tree_childs<value_t>
    childs(
        size_t index = 0) const
    {
      return {nodes[index].child, this};
    }
  };
}

#endif
