#include <lib/ios.hpp>
#include <lib/tree.hpp>



int main()
{
  lib::tree<int> t;


  auto parent = t.push_root(0);
  t.push_back_child(1, parent);
  t.push_back_child(22, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);
  t.push_back_child(3, parent);

  lib::printfln("#", (unsigned long long)&t);


  return 0;
}