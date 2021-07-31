#include <lib/ios.hpp>
#include <lib/vector.hpp>
#include <lib/test.hpp>

void should_initialize_empty_vector() 
{
  lib::vector<int> vec;
  test_equals(vec.size(), 0);
  test_equals(vec.capacity(), 10);
  test_equals(vec.begin(), vec.end());
}

void should_pushback_2_items()
{
  lib::vector<int> vec;
  vec.push_back(1);
  vec.push_back(1);
  test_equals(vec.size(), 2);
  test_equals(vec.capacity(), 10);
  test_equals(vec.begin() + 2, vec.end());
}

void should_reserve_1000_items()
{
  lib::vector<int> vec(1000);
  test_equals(vec.size(), 0);
  test_equals(vec.capacity(), 1000);
  test_equals(vec.begin(), vec.end());
}

void should_copy_vector();

void should_move_vector();

int main()
{
  run_test(should_initialize_empty_vector);
  run_test(should_pushback_2_items);
  run_test(should_reserve_1000_items);
}
