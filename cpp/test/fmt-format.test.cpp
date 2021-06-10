// #include <lib/format.hpp>
// #include <lib/test.hpp>
#define FMT_HEADER_ONLY
#include <fmt/format.h>

int main()
{
  fmt::format("The answer is {}.", 42);
}