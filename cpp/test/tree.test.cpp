#include <lib/ios.hpp>
#include <lib/tree.hpp>

template <typename value_t>
void print_tree(
    const lib::tree_node<value_t> &n)
{
  lib::printfln("# - #", n.id, n.value);

  for (const lib::tree_node<value_t> &sn : n.childs())
    print_tree(sn);
}

template <typename value_t>
void print_tree(
    const lib::tree<value_t> &t)
{
  print_tree(t[0]);
}

int main()
{
  lib::tree<int> t;

  t.push_root(20);
  t.push_child(22, 0);
  t.push_child(23, 1);
  t.push_child(21, 0);
  t.push_next(24, 1);

  print_tree(t);

  return 0;
}