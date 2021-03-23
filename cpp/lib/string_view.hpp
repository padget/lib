#ifndef __lib_string_view_hpp__
#define __lib_string_view_hpp__

#include <cstddef>

namespace lib
{
  template <typename char_t>
  class basic_string_view
  {
    const char_t *b = nullptr;
    std::size_t l = 0;

  public:
    constexpr basic_string_view() = default;

    constexpr basic_string_view(
        const char_t *s,
        const std::size_t n)
        : b(s), l(n) {}

    template <std::size_t n>
    constexpr basic_string_view(
        const char_t (&s)[n])
        : basic_string_view(s, n - 1) {}

    template <typename iterator_t>
    constexpr basic_string_view(
        iterator_t b, iterator_t e)
        : basic_string_view(&*b, e - b) {}

    constexpr basic_string_view(const basic_string_view &) = default;
    constexpr basic_string_view(basic_string_view &&) = default;

    ~basic_string_view() = default;

    basic_string_view &operator=(const basic_string_view &) = default;
    basic_string_view &operator=(basic_string_view &&) = default;

  public:
    const std::size_t &size() const { return l; }
    const char_t &operator[](const std::size_t &i) const { return *(b + i); }
    const char_t *begin() const { return b; }
    const char_t *end() const { return b + l; }

  public:
    bool start_with(basic_string_view o) const
    {
      if (l > o.l)
        return false;

      const char_t *bt = b;
      const char_t *et = bt + l;
      const char_t *bo = o.b;

      while (bt != et and *bt == *bo)
      {
        bo++;
        bt++;
      }

      return bo == o.b + o.l;
    }

    bool operator==(basic_string_view o) const
    {
      return l == o.l and start_with(o);
    }

    const char_t *find(basic_string_view o) const
    {
      using bsv = basic_string_view;

      auto *bt = b;
      auto *et = b + l;

      while (bt != et and not bsv(bt, et).start_with(o))
        bt++;

      return bt;
    }
  };

  using string_view = basic_string_view<char>;
  using wstring_view = basic_string_view<wchar_t>;
}

#endif