#include <lib/ios.hpp>
#include <lib/vector.hpp>

int main()
{
  lib::vector<char> v(10);
  v.push_back('0');
  v.push_back('0');
  v.push_back('0');
  v.push_back('0');

  const lib::vector<char> v2 = v;

  for (const char &c : v2)
  {
    lib::printf("#", c);
  }
}