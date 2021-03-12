#include <iostream>

#include "format.hpp"
#include "test.hpp"

using namespace clon::fmt;

void should_format()
{
  test_equals(format("{};{};{}", 1, 2, 3), "1;2;3");
  test_equals(format("{}", 1, 2, 3), "123");
  test_equals(format("{}{}{}-{}", 1, 2, 3), "123-");
  test_equals(format("{}{}{}{}", 1, 2, 3), "123");
  test_equals(format("{}_{}", std::string("co"), std::string("co")), "co_co");
  test_equals(format("{}", std::vector<char>({'c', 'o', 'u'})), "cou");
}

int main(int argc, char **argv)
{
  run_test(should_format);
}