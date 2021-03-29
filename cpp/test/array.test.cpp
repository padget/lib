#include <lib/test.hpp>
#include <lib/array.hpp>

void should_build_empty_array()
{
  lib::array<int, 1> arr;
  test_equals(arr.size(), 1);
  test_equals(arr.begin(), arr.data());
  test_equals(arr.end(), arr.begin() + arr.size());
}

void should_build_initialized_array()
{
  lib::array<int, 2> arr = {1, 2};
  test_equals(arr.size(), 2);
  test_equals(arr[0], 1);
  test_equals(arr[1], 2);
}

void should_be_equals_arrays()
{
  lib::array<int, 2> arr = {1, 2};
  lib::array<int, 2> arr2 = {1, 2};
  test_equals(arr, arr2);
}

int main()
{
  run_test(should_build_empty_array);
  run_test(should_build_initialized_array);
  run_test(should_be_equals_arrays);
}