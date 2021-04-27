#ifndef __clon_format_ext_hpp__
#define __clon_format_ext_hpp__

#include <lib/string_view.hpp>
#include <lib/string.hpp>
#include <lib/vector.hpp>
#include <lib/meta.hpp>

namespace lib
{
  template <charable char_t>
  class formatter_context;

  ///////////////////////////////////
  // lib::basic_string_view format //
  ///////////////////////////////////
  template <charable char_t>
  inline std::size_t length_of(
      const lib::basic_string_view<char_t> &v)
  {
    return v.size();
  }

  template <charable char_t>
  inline void format_of(
      formatter_context<char_t> &ctx,
      lib::basic_string_view<char_t> v)
  {
    for (auto &&c : v)
      ctx.append(c);
  }

  ///////////////////////////
  // literal string format //
  ///////////////////////////
  template <std::size_t n>
  inline std::size_t length_of(const char (&s)[n])
  {
    return n - 1;
  }

  template <std::size_t n>
  inline void format_of(
      formatter_context<char> &ctx,
      const char (&s)[n])
  {
    format_of(ctx, lib::basic_string_view<char>(s));
  }

  template <std::size_t n>
  inline std::size_t length_of(const wchar_t (&s)[n])
  {
    return n - 1;
  }

  template <std::size_t n>
  inline void format_of(
      formatter_context<wchar_t> &ctx,
      const wchar_t (&s)[n])
  {
    format_of(ctx, lib::basic_string_view<wchar_t>(s));
  }

  ///////////////////////////
  // integral types format //
  ///////////////////////////
  template <integer integral_t>
  inline std::size_t length_of(const integral_t &i)
  {
    if (i == 0)
      return 1;

    constexpr unsigned base(10);
    std::size_t len(0);
    integral_t tmp(i);

    while (tmp != 0)
    {
      tmp = tmp / base;
      len++;
    }

    return len;
  }

  template <charable char_t, integer integral_t>
  inline void format_of(
      formatter_context<char_t> &ctx,
      const integral_t &t)
  {
    if (t == 0)
      ctx.append('0');
    else
    {
      integral_t tmp(t);
      std::size_t cnt(0);

      while (tmp != 0)
      {
        ctx.append("0123456789"[tmp % 10]);
        tmp = tmp / 10;
        cnt++;
      }

      ctx.reverse(cnt);
    }
  }

  //////////////////////////
  // strings types format //
  //////////////////////////
  template <charable char_t>
  inline std::size_t length_of(const lib::basic_string<char_t> &s)
  {
    return length_of(lib::basic_string_view<char_t>(s.begin(), s.end()));
  }

  template <charable char_t>
  inline void format_of(
      formatter_context<char_t> &ctx,
      const lib::basic_string<char_t> &v)
  {
    format_of(ctx, lib::basic_string_view<char_t>(v.begin(), v.end()));
  }

  ////////////////////////
  // bools types format //
  ////////////////////////
  inline std::size_t length_of(const bool &s)
  {
    return s ? 4 : 5;
  }

  template <charable char_t>
  inline void format_of(
      formatter_context<char_t> &ctx,
      const bool &v)
  {
    format_of(ctx, v ? basic_string_view<char_t>("true")
                     : basic_string_view<char_t>("false"));
  }
  // //////////////////////////////////
  // // vector of chars types format //
  // //////////////////////////////////
  template <charable char_t>
  inline std::size_t length_of(const lib::vector<char_t> &v)
  {
    return length_of(lib::basic_string_view<char_t>(v.begin(), v.end()));
  }

  template <charable char_t>
  inline void format_of(
      formatter_context<char_t> &ctx,
      const lib::vector<char_t> &v)
  {
    format_of(ctx, lib::basic_string_view<char_t>(v.begin(), v.end()));
  }
}

#endif