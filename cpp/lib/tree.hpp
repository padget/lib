#ifndef __lib_tree_hpp__
#define __lib_tree_hpp__

#include <lib/exception.hpp>
#include <lib/vector.hpp>

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

    long unsigned first;
    tree<value_t> *t = nullptr;

    iterator
    begin()
    {
      return {first, t};
    }

    iterator
    end()
    {
      return {no_next, t};
    }

    const iterator
    begin() const
    {
      return {first, t};
    }

    const iterator
    end() const
    {
      return {no_next, t};
    }
  };

  template <
      typename value_t>
  struct tree_node
  {
    value_t value;
    tree<value_t> *tr = nullptr;
    unsigned id = 0;
    unsigned child = no_child;
    unsigned next = no_next;
    unsigned last_child = no_child;

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
    tree(unsigned _max)
        : nodes(_max) {}

  public:
    inline size_t size() const
    {
      return nodes.size();
    }

  public:
    inline unsigned
    push_root(
        const value_t &val)
    {
      if (nodes.empty())
        nodes.emplace_back(tree_node<value_t>{val, this});

      return nodes.back().id;
    }

    inline unsigned
    push_back_child(
        const value_t &value,
        unsigned parent_id)
    {
      if (parent_id == no_root)
        throw parent_index_doesnt_exist();

      if (parent_id > nodes.back_index())
        throw parent_index_doesnt_exist();

      tree_node<value_t> &parent = nodes[parent_id];
      nodes.emplace_back(tree_node<value_t>{value, this});

      tree_node<value_t> &nn = nodes.back();
      unsigned nnid = nodes.back_index();
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
        unsigned i) const
    {
      return nodes[i];
    }

    inline tree_node<value_t> &
    operator[](
        unsigned i)
    {
      return nodes[i];
    }

    inline tree_childs<value_t>
    childs(
        unsigned index = 0)
    {
      return {nodes[index].child, this};
    }

    inline const tree_childs<value_t>
    childs(
        unsigned index = 0) const
    {
      return {nodes[index].child, this};
    }
  };
}

#endif