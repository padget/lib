#include <lib/test.hpp>
#include <lib/array.hpp>

void should_build_empty_array()
{
  lib::array<int, 1> arr;
  arr.size();
  test_equals(arr.size(), 1);
  test_equals(arr.begin(), arr.data());
  test_equals(arr.end(), arr.begin() + arr.size());
}

void should_build_initialized_array()
{
}

void should_be_equals_arrays()
{
}

int main()
{
  run_test(should_build_empty_array);
  run_test(should_build_initialized_array);
  run_test(should_be_equals_arrays);
}