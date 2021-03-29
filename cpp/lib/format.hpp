#ifndef __clon_format_hpp__
#define __clon_format_hpp__

#include "string.hpp"
#include "vector.hpp"
#include "string_view.hpp"
#include "span.hpp"

#include "format-types.hpp"

namespace lib
{
  template <typename char_t>
  using view = lib::basic_string_view<char_t>;

  template <typename char_t>
  using buffer = lib::basic_string<char_t>;

  template <typename char_t>
  class formatter_context
  {
  private:
    buffer<char_t> &buff;

  public:
    explicit formatter_context(buffer<char_t> &_buff) : buff(_buff) {}

  public:
    inline void append(char_t c)
    {
      buff.push_back(c);
    }

    inline void reverse(std::size_t lastn)
    {
      char_t *end = buff.end();
      char_t *begin = buff.end() - lastn;
      span<char_t>(begin, end).reverse();
    }
  };

  template <typename char_t>
  class tokenizer
  {
    basic_string_view<char_t> data;
    char_t sep;

  public:
    tokenizer(basic_string_view<char_t> _data, char_t _sep)
        : data(_data), sep(_sep) {}

  public:
    inline basic_string_view<char_t> next()
    {
      basic_string_view<char_t> n(data.begin(), data.find(sep));
      data = basic_string_view<char_t>(n.end() == data.end() ? n.end() : n.end() + 1, data.end());
      return n;
    }

    inline basic_string_view<char_t> tail() { return data; }
  };

  template <typename char_t, typename... args_t>
  inline void __format_into(
      formatter_context<char_t> &ctx,
      view<char_t> fmt, const args_t &...args)
  {
    tokenizer<char_t> tk(fmt, '#');
    ((format_of(ctx, tk.next()), format_of(ctx, args)), ...);
    format_of(ctx, tk.tail());
  }

  template <typename char_t, typename... args_t>
  inline std::size_t __all_length_of(view<char_t> fmt, const args_t &...args)
  {
    return (fmt.count_if([](const char_t &c) { return c != '#'; }) + ... + length_of(args));
  }

  template <typename char_t, typename... args_t>
  inline lib::basic_string<char_t> __format(view<char_t> fmt, const args_t &...args)
  {
    buffer<char_t> buff(__all_length_of(fmt, args...));
    formatter_context<char_t> ctx{buff};
    __format_into(ctx, fmt, args...);
    return buff;
  }

  template <typename... args_t>
  inline lib::basic_string<char> format(
      view<char> fmt, const args_t &...args)
  {
    return __format(fmt, args...);
  }

  template <typename... args_t>
  inline lib::basic_string<wchar_t> format(
      view<wchar_t> fmt, const args_t &...args)
  {
    return __format(fmt, args...);
  }

  template <typename... args_t>
  inline std::size_t all_length_of(
      view<char> fmt, const args_t &...args)
  {
    return __all_length_of(fmt, args...);
  }

  template <typename... args_t>
  inline std::size_t all_length_of(
      view<wchar_t> fmt, const args_t &...args)
  {
    return __all_length_of(fmt, args...);
  }

  template <typename... args_t>
  inline void format_into(
      formatter_context<char> &ctx,
      view<char> fmt, const args_t &...args)
  {
    __format_into(ctx, fmt, args...);
  }

  template <typename... args_t>
  inline void format_into(
      formatter_context<wchar_t> &ctx,
      view<wchar_t> fmt, const args_t &...args)
  {
    __format_into(ctx, fmt, args...);
  }
} // namespace clon::fmt

#endif
