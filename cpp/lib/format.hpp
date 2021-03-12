#ifndef __clon_format_hpp__
#define __clon_format_hpp__

#include <iostream>

#include <array>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string_view>
#include <tuple>

#include "format-types.hpp"

namespace clon::fmt
{
  template <typename char_t>
  using view = std::basic_string_view<char_t>;

  template <typename char_t, std::size_t n>
  using views = std::array<view<char_t>, n>;

  template <typename char_t>
  using buffer = std::basic_string<char_t>;

  template <typename char_t>
  class formatter_context
  {
  private:
    buffer<char_t> &buff;

  public:
    explicit formatter_context(buffer<char_t> &_buff) : buff(_buff) {}

  public:
    void append(const char_t &c)
    {
      buff.push_back(c);
    }

    auto end()
    {
      return buff.end();
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

    const std::size_t &length() const
    {
      if (len == 0)
        len = length_of(data);

      return len;
    }

    template <typename char_t>
    void format_into(formatter_context<char_t> &ctx) const
    {
      format_of(ctx, data);
    }
  };

  template <typename type_t>
  basic_format<type_t> make_format(const type_t &data)
  {
    return basic_format<type_t>(data);
  }

  template <typename char_t, std::size_t n>
  struct pattern
  {
  private:
    view<char_t> fmt;
    view<char_t> sep;
    std::size_t _parts_size = 0;
    views<char_t, n> _parts;

  public:
    explicit pattern(view<char_t> _fmt, view<char_t> _sep)
        : fmt(_fmt), sep(_sep)
    {
      auto bf = fmt.begin(), ef = fmt.end();
      auto bs = sep.begin(), es = sep.end();

      for (view<char_t> &part : _parts)
      {
        auto found = std::search(bf, ef, bs, es);
        part = view<char_t>(bf, found);

        if (found != ef)
          bf = (found + sep.size());

        _parts_size += part.size();
      }
    }

  public:
    const std::size_t &parts_size() const
    {
      return _parts_size;
    }

    std::size_t full_size() const
    {
      return fmt.size();
    }

    const views<char_t, n> &parts() const
    {
      return _parts;
    }
  };

  template <typename char_t, std::size_t n>
  std::size_t length_of(const pattern<char_t, n> &p)
  {
    return p.parts_size();
  }

  template <typename... args_t>
  using basics = std::tuple<basic_format<args_t>...>;

  template <typename char_t, typename... args_t>
  class partial_formatter
  {
    constexpr static view<char_t> sep = "{}";
    pattern<char_t, sizeof...(args_t) + 1> p;
    basics<args_t...> bcs;

  public:
    explicit partial_formatter(view<char_t> fmt, const args_t &...args)
        : p(fmt, sep), bcs(make_format(args)...) {}

  public:
    const std::size_t length() const
    {
      constexpr auto sum = [](auto &&...a) { return (a.length() + ... + 0); };
      return p.parts_size() + std::apply(sum, bcs);
    }

    void format(formatter_context<char_t> &ctx) const
    {
      std::apply([&parts = p.parts(), &ctx](auto &&...args) {
        std::size_t i(0);
        ((make_format(parts.at(i++)).format_into(ctx), args.format_into(ctx)), ...);
        make_format(parts.at(i)).format_into(ctx);
      },
                 bcs);
    }
  };

  template <typename char_t, typename... args_t>
  partial_formatter<char_t, args_t...> make_partial(
      view<char_t> fmt, const args_t &...args)
  {
    return partial_formatter<char_t, args_t...>(fmt, args...);
  }

  template <typename char_t, typename... args_t>
  class formatter
  {
    partial_formatter<char_t, args_t...> partial;

  public:
    explicit formatter(view<char_t> fmt, const args_t &...args) : partial(fmt, args...) {}

  public:
    buffer<char_t> format()
    {
      buffer<char_t> buff;
      buff.reserve(partial.length());

      formatter_context<char_t> ctx{buff};
      partial.format(ctx);

      return buff;
    }
  };

  template <typename char_t, typename... args_t>
  formatter<char_t, args_t...> make_formatter(
      view<char_t> fmt, const args_t &...args)
  {
    return formatter<char_t, args_t...>(fmt, args...);
  }

  template <typename char_t>
  std::basic_string<char_t>
  to_string(buffer<char_t> &&b)
  {
    return std::basic_string(b.data(), b.size());
  }

  template <typename... args_t>
  buffer<char> format(
      view<char> fmt, const args_t &...args)
  {
    return make_formatter(fmt, args...).format();
  }

  template <typename... args_t>
  buffer<wchar_t> format(
      view<wchar_t> fmt, const args_t &...args)
  {
    return make_formatter(fmt, args...).format();
  }

  template <typename... args_t>
  std::size_t predict_length_of(
      view<char> fmt, const args_t &...args)
  {
    return make_partial(fmt, args...).length();
  }

  template <typename... args_t>
  std::size_t predict_length_of(
      view<wchar_t> fmt, const args_t &...args)
  {
    return make_partial(fmt, args...).length();
  }

  template <typename... args_t>
  void format_into(
      formatter_context<char> &ctx,
      view<char> fmt, const args_t &...args)
  {
    make_partial(fmt, args...).format(ctx);
  }

  template <typename... args_t>
  void format_into(
      formatter_context<wchar_t> &ctx,
      view<wchar_t> fmt, const args_t &...args)
  {
    make_partial(fmt, args...).format(ctx);
  }
} // namespace clon::fmt

#endif
