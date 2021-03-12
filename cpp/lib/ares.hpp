#ifndef __ares_hpp__
#define __ares_hpp__

#include <string>
#include <string_view>

namespace ares
{
  template <typename char_t>
  std::basic_string<char_t> encrypt(std::basic_string_view<char_t> s)
  {
    return std::basic_string<char_t>(s.begin(), s.end());
  }

  template <typename char_t>
  std::basic_string<char_t> decrypt(std::basic_string_view<char_t> s)
  {
    return std::basic_string<char_t>(s.begin(), s.end());
  }
}

#endif