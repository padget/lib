#include <lib/ios.hpp>
#include <lib/tree.hpp>

template <typename value_t>
void print_tree(
    const lib::tree_node<value_t> &n)
{
  lib::printfln("i # - v # - n # - c #", n.id, n.value, n.next, n.child);

  for (const lib::tree_node<value_t> &sn : n.childs())
    lib::printfln("#", sn.id);
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

  auto parent = t.push_root(0);
  t.push_back_child(1, parent);
  t.push_back_child(2, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);

  lib::printfln("#", (unsigned)&t);

  print_tree(t);

  return 0;
}