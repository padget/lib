#include <lib/ios.hpp>
#include <lib/vector.hpp>

int main()
{
  lib::vector<char> v(4);
  v.push_back('0');

  for (auto c : v)
    lib::printfln("#", c);

  lib::printfln("vector size #", v.size());
}