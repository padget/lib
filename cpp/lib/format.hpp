#ifndef __clon_format_hpp__
#define __clon_format_hpp__

#include "array.hpp"
#include "string.hpp"
#include "vector.hpp"
#include "string_view.hpp"
#include "scanner.hpp"
#include "span.hpp"

#include "format-types.hpp"

namespace lib
{
  template <typename char_t>
  using view = lib::basic_string_view<char_t>;

  template <typename char_t, std::size_t n>
  using views = lib::array<view<char_t>, n>;

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
    inline void append(const char_t &c)
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

  template <typename char_t, typename... args_t>
  inline void __format_into(
      formatter_context<char_t> &ctx,
      view<char_t> fmt, const args_t &...args)
  {
    array<view<char_t>, sizeof...(args_t) + 1> parts;
    basic_scanner<char_t> scan(fmt);

    for (view<char_t> &part : parts)
    {
      scan.until('#');
      part = scan.extract();
      scan.advance();
      scan.ignore();
    }

    std::size_t i(0);
    ((format_of(ctx, parts[i++]), format_of(ctx, args)), ...);
    format_of(ctx, parts[i]);
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
