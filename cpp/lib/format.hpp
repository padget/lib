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

  template <typename type_t>
  class basic_format
  {
    const type_t &data;
    mutable std::size_t len = 0;

  public:
    explicit basic_format(const type_t &_data)
        : data(_data) {}

    inline const std::size_t &length() const
    {
      if (len == 0)
        len = length_of(data);

      return len;
    }

    template <typename char_t>
    inline void format_into(formatter_context<char_t> &ctx) const
    {
      format_of(ctx, data);
    }
  };

  template <typename type_t>
  inline basic_format<type_t> make_format(const type_t &data)
  {
    return basic_format<type_t>(data);
  }

  template <typename char_t, std::size_t n>
  struct pattern
  {
    constexpr static char_t sep = '#';

  private:
    view<char_t> fmt;
    std::size_t _parts_size = 0;
    views<char_t, n> _parts;

  public:
    explicit pattern(view<char_t> _fmt)
        : fmt(_fmt)
    {
      basic_scanner<char_t> scan(fmt);

      for (view<char_t> &part : _parts)
      {
        scan.until(sep);
        part = scan.extract();
        scan.advance();
        scan.ignore();
        _parts_size += part.size();
      }
    }

  public:
    inline const std::size_t &parts_size() const
    {
      return _parts_size;
    }

    inline std::size_t full_size() const
    {
      return fmt.size();
    }

    inline const views<char_t, n> &parts() const
    {
      return _parts;
    }
  };

  template <typename char_t, typename... args_t>
  inline std::size_t __all_length_of(
      const pattern<char_t, sizeof...(args_t) + 1> &p,
      const args_t &...args)
  {
    return (p.parts_size() + ... + make_format(args).length());
  }

  template <typename char_t, typename... args_t>
  inline std::size_t __all_length_of(view<char_t> fmt, const args_t &...args)
  {
    return __all_length_of(pattern<char_t, sizeof...(args_t) + 1>(fmt), args...);
  }

  template <typename char_t, typename... args_t>
  inline void __format_into(
      const pattern<char_t, sizeof...(args_t) + 1> &p,
      formatter_context<char_t> &ctx,
      const args_t &...args)
  {
    std::size_t i(0);
    ((make_format(p.parts()[i++]).format_into(ctx), make_format(args).format_into(ctx)), ...);
    make_format(p.parts()[i]).format_into(ctx);
  }

  template <typename char_t, typename... args_t>
  inline void __format_into(
      formatter_context<char_t> &ctx,
      view<char_t> fmt, const args_t &...args)
  {
    __format_into(pattern<char_t, sizeof...(args_t) + 1>(fmt), ctx, args...);
  }

  template <typename char_t, typename... args_t>
  inline buffer<char_t> __format(view<char_t> fmt, const args_t &...args)
  {
    buffer<char_t> buff;
    formatter_context<char_t> ctx{buff};
    __format_into(ctx, fmt, args...);
    return buff;
  }

  template <typename... args_t>
  inline buffer<char> format(
      view<char> fmt, const args_t &...args)
  {
    return __format(fmt, args...);
  }

  template <typename... args_t>
  inline buffer<wchar_t> format(
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
