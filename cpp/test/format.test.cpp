#include <lib/format.hpp>
#include <lib/format-ansi.hpp>
#include <lib/ios.hpp>

int main()
{
  lib::printf("rpouthis is the answer #", lib::red | lib::bold | 42);
}
