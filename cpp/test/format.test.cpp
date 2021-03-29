#include <lib/format.hpp>
#include <lib/test.hpp>

void should_format()
{
  test_equals(lib::format("#;#;#", 1, 2, 3), "1;2;3");
  test_equals(lib::format("#", 1, 2, 3), "123");
  test_equals(lib::format("###-#", 1, 2, 3), "123-#");
  test_equals(lib::format("####", 1, 2, 25553), "1225553#");
  test_equals(lib::format("#_#", lib::string("co"), lib::string("co")), "co_co");
  // test_equals(lib::format("#", lib::vector<char>("cou")), "cou");
}

int main(int argc, char **argv)
{
  run_test(should_format);
}