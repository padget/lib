#include <iostream>
#include <lib/vector.hpp>

int main()
{
  lib::vector<char> v(4);
  v.push_back('1');
  v.push_back('2');
  v.push_back('3');
  v.push_back('\0');
  std::cout << std::boolalpha << (v == "123");

  std::cout << "vector test\n";
}