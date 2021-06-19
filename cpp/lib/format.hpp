#ifndef __clon_format_hpp__
#define __clon_format_hpp__

#include <lib/string.hpp>
#include <lib/string_view.hpp>
#include <lib/meta.hpp>
#include <lib/algorithm.hpp>
#include <lib/file.hpp>
#include <lib/array.hpp>
namespace lib
{
  template <character char_t, typename buffer_t>
  class formatter_context;

  ///////////////////////////////////
  // lib::basic_string_view format //
  ///////////////////////////////////
  template <character char_t>
  inline std::size_t length_of(
      const lib::basic_string_view<char_t> &v)
  {
    return v.size();
  }

  template <character char_t, typename buffer_t>
  inline void format_of(
      formatter_context<char_t, buffer_t> &ctx,
      lib::basic_string_view<char_t> v)
  {
    for (auto &&c : v)
      ctx.push_back(c);
  }

  ///////////////////////////
  // literal string format //
  ///////////////////////////
  template <std::size_t n>
  inline std::size_t length_of(const char (&s)[n])
  {
    return n - 1;
  }

  template <std::size_t n, typename buffer_t>
  inline void format_of(
      formatter_context<char, buffer_t> &ctx,
      const char (&s)[n])
  {
    format_of(ctx, lib::basic_string_view<char>(s));
  }

  template <std::size_t n>
  inline std::size_t length_of(const wchar_t (&s)[n])
  {
    return n - 1;
  }

  template <std::size_t n, typename buffer_t>
  inline void format_of(
      formatter_context<wchar_t, buffer_t> &ctx,
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
    const integral_t v = i;
    // const int abs[2] = {i, -i};
    // const integral_t v = abs[i < 0];

    if (v < 10)
      return 1;
    else if (v < 100)
      return 2;
    else if (v < 1000)
      return 3;
    else if (v < 10000)
      return 4;
    else if (v < 100000)
      return 5;
    else if (v < 1000000)
      return 5;
    else if (v < 10000000)
      return 7;
    else if (v < 100000000)
      return 8;
    else if (v < 1000000000)
      return 9;
    else if (v < 10000000000)
      return 10;
    else if (v < 100000000000)
      return 11;
    else if (v < 1000000000000)
      return 12;
    else if (v < 10000000000000)
      return 13;
    else if (v < 100000000000000)
      return 14;
    else if (v < 1000000000000000)
      return 15;
    else if (v < 10000000000000000)
      return 136;
    else if (v < 100000000000000000)
      return 17;
    else if (v < 1000000000000000000)
      return 18;
    else
      return 1;
  }
  template <character char_t, integer integral_t, typename buffer_t>
  inline void format_of(
      formatter_context<char_t, buffer_t> &ctx,
      const integral_t &t)
  {
    if (t == 0)
      ctx.push_back('0');
    else
    {
      array<char_t, 100> buff;
      auto b = buff.begin();
      auto s = b;
      integral_t tmp(t);

      while (tmp != 0)
      {
        *b = "0123456789"[tmp % 10];
        b = b + 1;
        tmp = tmp / 10;
      }

      lib::reverse(s, b);
      lib::for_each(
          s, b, [&ctx](char_t c)
          { ctx.push_back(c); });
    }
  }

  //////////////////////////
  // strings types format //
  //////////////////////////
  template <character char_t>
  inline std::size_t length_of(const lib::basic_string<char_t> &s)
  {
    return length_of(lib::basic_string_view<char_t>(s.begin(), s.end()));
  }

  template <character char_t, typename buffer_t>
  inline void format_of(
      formatter_context<char_t, buffer_t> &ctx,
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

  template <character char_t, typename buffer_t>
  inline void format_of(
      formatter_context<char_t, buffer_t> &ctx,
      const bool &v)
  {
    format_of(ctx, v ? basic_string_view<char_t>("true")
                     : basic_string_view<char_t>("false"));
  }
  // //////////////////////////////////
  // // vector of chars types format //
  // //////////////////////////////////
  template <character char_t>
  inline std::size_t length_of(const lib::vector<char_t> &v)
  {
    return length_of(lib::basic_string_view<char_t>(v.begin(), v.end()));
  }

  template <character char_t, typename buffer_t>
  inline void format_of(
      formatter_context<char_t, buffer_t> &ctx,
      const lib::vector<char_t> &v)
  {
    format_of(ctx, lib::basic_string_view<char_t>(v.begin(), v.end()));
  }
}

namespace lib
{
  template <character char_t>
  using view = lib::basic_string_view<char_t>;

  template <
      character char_t,
      typename buffer_t>
  class formatter_context
  {
  private:
    buffer_t &buff;

  public:
    explicit formatter_context(
        buffer_t &_buff)
        : buff(_buff) {}

  public:
    inline void
    push_back(char_t c)
    {
      buff.push_back(c);
    }
  };

  namespace __fmt
  {
    template <character char_t, char_t sep>
    class tokenizer
    {
      basic_string_view<char_t> data;

    public:
      tokenizer(
          basic_string_view<char_t> _data)
          : data(_data) {}

    public:
      inline basic_string_view<char_t>
      next()
      {
        basic_string_view<char_t> n(data.begin(), lib::find(data.begin(), data.end(), sep));
        data = basic_string_view<char_t>(n.end() == data.end() ? n.end() : n.end() + 1, data.end());
        return n;
      }

      inline basic_string_view<char_t>
      tail()
      {
        return data;
      }
    };

    template <
        character char_t,
        typename buffer_t,
        typename... args_t>
    inline void
    format_into(
        formatter_context<char_t, buffer_t> &ctx,
        view<char_t> fmt, const args_t &...args)
    {
      tokenizer<char_t, '#'> tk(fmt);
      ((format_of(ctx, tk.next()), format_of(ctx, args)), ...);
      format_of(ctx, tk.tail());
    }

    template <
        character char_t,
        typename... args_t>
    inline std::size_t
    all_length_of(
        view<char_t> fmt,
        const args_t &...args)
    {
      return (fmt.size() + ... + length_of(args));
    }

    template <
        character char_t,
        typename... args_t>
    inline lib::basic_string<char_t>
    format(view<char_t> fmt,
           const args_t &...args)
    {
      basic_string<char_t> buff(all_length_of(fmt, args...));
      formatter_context<char_t, basic_string<char_t>> ctx{buff};
      format_into(ctx, fmt, args...);
      return buff;
    }
  }

  template <typename... args_t>
  inline lib::basic_string<char>
  format(
      view<char> fmt,
      const args_t &...args)
  {
    return __fmt::format(fmt, args...);
  }

  template <typename... args_t>
  inline lib::basic_string<wchar_t>
  format(
      view<wchar_t> fmt,
      const args_t &...args)
  {
    return __fmt::format(fmt, args...);
  }

  template <typename... args_t>
  inline std::size_t
  all_length_of(
      view<char> fmt,
      const args_t &...args)
  {
    return __fmt::all_length_of(fmt, args...);
  }

  template <typename... args_t>
  inline std::size_t
  all_length_of(
      view<wchar_t> fmt,
      const args_t &...args)
  {
    return __fmt::all_length_of(fmt, args...);
  }

  template <typename buffer_t, typename... args_t>
  inline void
  format_into(
      formatter_context<char, buffer_t> &ctx,
      view<char> fmt, const args_t &...args)
  {
    __fmt::format_into(ctx, fmt, args...);
  }

  template <typename buffer_t, typename... args_t>
  inline void format_into(
      formatter_context<wchar_t, buffer_t> &ctx,
      view<wchar_t> fmt, const args_t &...args)
  {
    __fmt::format_into(ctx, fmt, args...);
  }

  template <typename... args_t>
  void format(file &out, string_view fmt, const args_t &...args)
  {
    formatter_context<char, file> ctx(out);
    format_into(ctx, fmt, args...);
  }

  template <typename... args_t>
  void format(file &out, wstring_view fmt, const args_t &...args)
  {
    formatter_context<wchar_t, file> ctx(out);
    format_into(ctx, fmt, args...);
  }
} // namespace lib

#endif
