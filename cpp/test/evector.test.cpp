#include <lib/ios.hpp>
#include <lib/evector.hpp>

int main()
{
  lib::evector<char> v(10);
  v.push_back('0');
  v.push_back('0');
  v.push_back('0');

  for (auto c : v)
    lib::printfln("#", c);

  lib::printfln("vector size # #", v.size(), v.index.size());

}