#include <lib/ios.hpp>
#include <lib/tree.hpp>

int main()
{
  lib::tree<int, int> t;
  t.push(1, 20, lib::no_root, lib::index_type::parent);
  t.push(1, 21, 1, lib::index_type::parent);
  t.push(1, 22, 2, lib::index_type::pred);
  t.push(1, 23, 3, lib::index_type::pred);

  for (auto&& n : t.childs_of(1))
    lib::printfln("#", n);
  
  for (std::size_t i(0); i <= t.size(); ++i)
    lib::printfln("index : #, next : #, child : #, value : #", i, t[i].next, t[i].child, t[i].value);

  return 0;
}