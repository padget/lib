#include <lib/format.hpp>
#include <lib/ios.hpp>

int main()
{
  lib::printf(lib::string_view("this is the answer  d#"), 42);
}

// #include <algorithm>
// #include <iostream>

// template <typename char_t, size_t N>
// struct StringLiteral
// {
//   constexpr StringLiteral(const char_t (&str)[N])
//   {
//     std::copy_n(str, N, value);
//   }

//   char_t value[N];
// };

// template <StringLiteral lit>
// void Print()
// {
//   // The size of the string is available as a constant expression.
//   constexpr auto size = sizeof(lit.value);

//   // and so is the string's content.
//   constexpr auto contents = lit.value;

//   std::cout << "Size: " << size << ", Contents: " << contents << std::endl;
// }

// int main()
// {
//   Print<"literal string">(); // Prints "Size: 15, Contents: literal string"
// }