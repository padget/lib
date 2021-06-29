#include <lib/utility.hpp>
#include <cstring>
#include <chrono>
#include <lib/ios.hpp>

int main()
{
  int src[100000] = {0};

  for (int i = 0; i < 100000; ++i)
    src[i] = i;

  int des[100000] = {1};

  auto start_time1 = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 100000; ++i)
    lib::memcpy(src, src + 100000, des);

  auto end_time1 = std::chrono::high_resolution_clock::now();
  auto time1 = end_time1 - start_time1;
  lib::printfln("lib::memcpy took # ms to run : #", time1 / std::chrono::milliseconds(1), des[0]);

  int src2[100000] = {0};

  for (int i = 0; i < 100000; ++i)
    src2[i] = i;

  int des2[100000] = {1};

  auto start_time2 = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 100000; ++i)
    std::memcpy(des2, src2, 100000 * sizeof(int));

  auto end_time2 = std::chrono::high_resolution_clock::now();
  auto time2 = end_time2 - start_time2;
  lib::printfln("std::memcpy took # ms to run : #", time2 / std::chrono::milliseconds(1), des2[0]);

  return des[0];
}