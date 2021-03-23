#ifndef __lib_scanner_hpp__
#define __lib_scanner_hpp__

#include "string_view.hpp"

namespace lib
{
  template <typename char_t>
  class basic_scanner
  {
    lib::basic_string_view<char_t> data;
    std::size_t index = 0;
    std::size_t prev = 0;

  public:
    explicit basic_scanner(
        basic_string_view<char_t> sv)
        : data{sv} {}

  public:
    void until(char_t c)
    {
      while (index < data.size() and data[index] != c)
        advance();
    }

    void advance(std::size_t step = 1)
    {
      if (index + step <= data.size())
        index += step;
    }

    void ignore() { prev = index; }

    basic_string_view<char_t> extract()
    {
      const char_t *b = &data[prev];
      std::size_t l = index - prev;

      basic_string_view<char_t> ext(b, l);
      prev = index;
      return ext;
    }
  };

  using scanner = basic_scanner<char>;
  using wscanner = basic_scanner<wchar_t>;

}

#endif