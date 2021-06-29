#ifndef __lib_scanner_hpp__
#define __lib_scanner_hpp__

#include <lib/string_view.hpp>
#include <lib/algorithm.hpp>

namespace lib
{
  template <character char_t>
  class basic_scanner
  {
    lib::basic_string_view<char_t> data;
    size_t index = 0;
    size_t prev = 0;

  public:
    constexpr explicit basic_scanner(
        basic_string_view<char_t> sv)
        : data{sv} {}

  public:
    constexpr bool starts_with(basic_string_view<char_t> sv) const
    {
      return lib::starts_with(
          data.begin(), data.end(),
          sv.begin(), sv.end());
    }

    constexpr void until(char_t c)
    {
      while (index < data.size() and data[index] != c)
        advance();
    }

    constexpr bool finished() const
    {
      return index == data.size();
    }

    constexpr bool between(char_t min, char_t max) const
    {
      return min <= data[index] and data[index] <= max;
    }

    constexpr bool is(char_t c) const
    {
      return data[index] == c;
    }

    template <character... chars_t>
    constexpr bool in(chars_t... c) const
    {
      return ((data[index] == c) || ... || false);
    }

    constexpr void advance(size_t step = 1)
    {
      if (index + step <= data.size())
        index += step;
    }

    constexpr void ignore() { prev = index; }

    constexpr basic_string_view<char_t> extract()
    {
      const char_t *b = &data[prev];
      unsigned l = index - prev;

      basic_string_view<char_t> ext(b, l);
      prev = index;
      return ext;
    }

    constexpr basic_string_view<char_t> tail()
    {
      return basic_string_view<char_t>(&data[index], data.end());
    }
  };

  using scanner = basic_scanner<char>;
  using wscanner = basic_scanner<wchar_t>;
}

#endif