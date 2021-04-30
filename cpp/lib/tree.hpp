#ifndef __lib_tree_hpp__
#define __lib_tree_hpp__

#include <lib/exception.hpp>
#include <lib/vector.hpp>

namespace lib
{
  BASIC_EXCEPTION(tree_exception)
  INHERITED_EXCEPTION(parent_index_doesnt_exist, tree_exception)
  INHERITED_EXCEPTION(previous_index_doesnt_exist, tree_exception)

  constexpr std::size_t no_next = static_cast<std::size_t>(-1);
  constexpr std::size_t no_child = static_cast<std::size_t>(-1);
  constexpr std::size_t no_root = static_cast<std::size_t>(-1);

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
    std::size_t index;
    tree<value_t> &t;

    inline tree_node<value_t> &
    operator*()
    {
      return t[index];
    }

    inline const tree_node<value_t> &
    operator*() const
    {
      return t[index];
    }

    inline child_iterator &
    operator++()
    {
      if (index != no_next)
        index = t[index].next;

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
    operator!=(
        const child_iterator o)
    {
      return index != o.index;
    }
  };

  template <
      typename value_t>
  struct tree_childs
  {
    using iterator = child_iterator<value_t>;

    std::size_t index;
    tree<value_t> &t;

    iterator
    begin()
    {
      return {index, t};
    }

    iterator
    end()
    {
      return {no_next, t};
    }

    const iterator
    begin() const
    {
      return {index, t};
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
    std::size_t id = 0;
    std::size_t child = no_child;
    std::size_t next = no_next;
    std::size_t last_child = no_child;

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
      return {child, *tr};
    }

    const tree_childs<value_t>
    childs() const
    {
      return {child, *tr};
    }
  };

  template <
      typename value_t>
  class tree
  {
    vector<tree_node<value_t>> nodes;

  public:
    tree() = default;
    tree(const tree &) = default;
    tree(tree &&) = default;
    tree &operator=(const tree &) = default;
    tree &operator=(tree &&) = default;

  public:
    tree(std::size_t _max)
        : nodes(_max) {}

  public:
    inline size_t size() const
    {
      return nodes.size();
    }

  public:
    inline std::size_t
    push_root(
        const value_t &val)
    {
      if (nodes.empty())
        nodes.emplace_back(
            tree_node<value_t>{val, this});

      return nodes.back().id;
    }

    inline std::size_t
    push_front_child(
        const value_t &value,
        std::size_t index)
    {
      if (index == no_root)
        throw parent_index_doesnt_exist();

      if (index > nodes.back_index())
        throw parent_index_doesnt_exist();

      nodes.emplace_back(
          tree_node<value_t>{value, this});

      tree_node<value_t> &newnode = nodes.back();
      std::size_t newchild = nodes.back_index();
      newnode.id = newchild;

      tree_node<value_t> &parent = nodes[index];
      std::size_t oldchild = parent.child;

      parent.child = newchild;
      newnode.next = oldchild;

      if (parent.child == no_child)
        parent.last_child = oldchild;

      return newchild;
    }

    inline std::size_t
    push_back_child(
        const value_t &value,
        std::size_t parent_id)
    {
      if (parent_id == no_root)
        throw parent_index_doesnt_exist();

      if (parent_id > nodes.back_index())
        throw parent_index_doesnt_exist();

      tree_node<value_t> &parent = nodes[parent_id];
      nodes.emplace_back(tree_node<value_t>{value, this});

      tree_node<value_t> &nn = nodes.back();
      std::size_t nnid = nodes.back_index();
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
        std::size_t i) const
    {
      return nodes[i];
    }

    inline tree_node<value_t> &
    operator[](
        std::size_t i)
    {
      return nodes[i];
    }

    inline tree_childs<value_t>
    childs_of(
        std::size_t index = 0)
    {
      return {nodes[index].child, *this};
    }

    inline const tree_childs<value_t>
    childs_of(
        std::size_t index = 0) const
    {
      return {nodes[index].child, *this};
    }
  };
}

#endif