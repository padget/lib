#include <lib/ios.hpp>
#include <lib/tree.hpp>

int main()
{
  lib::tree<int, int> t;

  t.push_root(1, 20);
  t.push_child(1, 22, 0);
  t.push_child(1, 23, 1);
  t.push_child(1, 21, 0);
  t.push_next(1, 24, 1);

  for (auto &&n : t.childs_of(0))
    lib::printfln("child of 0 #", n);

  for (auto &&n : t.childs_of(1))
    lib::printfln("child of 1 #", n);

  return 0;
}