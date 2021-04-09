#include <lib/tree.hpp>
#include <lib/ios.hpp>

int main()
{
  lib::tree<int, int> t;
  t.push(1, 2, lib::no_root, lib::index_type::parent);
  t.push(1, 2, 1, lib::index_type::parent);
  t.push(1, 2, 2, lib::index_type::pred);
  t.push(1, 2, 3, lib::index_type::pred);

  for (auto&& n : t.childs(1))
    lib::print(n);
  
  return 0;
}