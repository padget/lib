#include <lib/ios.hpp>
#include <lib/vector.hpp>
#include <lib/test.hpp>

void should_initialize_empty_vector();
void should_pushback_2_items();
void should_reserve_1000_items();
void should_copy_vector();
void should_move_vector();

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
