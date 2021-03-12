#ifndef __clon_format_ext_hpp__
#define __clon_format_ext_hpp__

#include <string_view>
#include <vector>
#include <concepts>

namespace clon::fmt
{
  template <typename char_t>
  struct formatter_context;

  template <typename char_t>
  concept charable =
      std::same_as<char_t, char> or
      std::same_as<char_t, wchar_t>;

  ///////////////////////////////////
  // std::basic_string_view format //
  ///////////////////////////////////
  template <typename char_t>
  std::size_t length_of(
      const std::basic_string_view<char_t> &v)
  {
    return v.size();
  }

  template <typename char_t>
  void format_of(
      formatter_context<char_t> &ctx,
      const std::basic_string_view<char_t> &v)
  {
    for (const char_t &c : v)
      ctx.append(c);
  }

  ///////////////////////////
  // literal string format //
  ///////////////////////////
  template <std::size_t n>
  std::size_t length_of(const char (&s)[n])
  {
    return n - 1;
  }

  template <std::size_t n>
  void format_of(
      formatter_context<char> &ctx,
      const char (&s)[n])
  {
    format_of(ctx, std::basic_string_view<char>(s));
  }

  template <std::size_t n>
  std::size_t length_of(const wchar_t (&s)[n])
  {
    return n - 1;
  }

  template <std::size_t n>
  void format_of(
      formatter_context<wchar_t> &ctx,
      const wchar_t (&s)[n])
  {
    format_of(ctx, std::basic_string_view<wchar_t>(s));
  }

  ///////////////////////////
  // integral types format //
  ///////////////////////////
  template <std::integral integral_t>
  std::size_t length_of(const integral_t &i)
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

  template <typename char_t, std::integral integral_t>
  void format_of(
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

      std::reverse(ctx.end() - cnt, ctx.end());
    }
  }

  //////////////////////////
  // strings types format //
  //////////////////////////
  template <typename char_t>
  std::size_t length_of(const std::basic_string<char_t> &s)
  {
    return length_of(std::basic_string_view<char_t>(s.begin(), s.end()));
  }

  template <typename char_t>
  void format_of(
      formatter_context<char_t> &ctx,
      const std::basic_string<char_t> &v)
  {
    format_of(ctx, std::basic_string_view<char_t>(v.begin(), v.end()));
  }


  //////////////////////////////////
  // vector of chars types format //
  //////////////////////////////////
  template <charable char_t>
  std::size_t length_of(const std::vector<char_t> &v)
  {
    return length_of(std::basic_string_view<char_t>(v.begin(), v.end()));
  }

  template <charable char_t>
  void format_of(
      formatter_context<char_t> &ctx,
      const std::vector<char_t> &v)
  {
    format_of(ctx, std::basic_string_view<char_t>(v.begin(), v.end()));
  }
}

#endif