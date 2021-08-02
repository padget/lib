#include <lib/format.hpp>
#include <lib/format-ansi.hpp>
#include <lib/ios.hpp>

int main()
{
  lib::printf("rpouthis is the answer #", lib::cleft | lib::cleft | lib::bold | 42);
}
