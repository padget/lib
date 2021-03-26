#ifndef __lib_span_hpp__
#define __lib_span_hpp__

#include <cstddef>

namespace lib
{
  namespace detail
  {
    template <typename iterator>
    iterator find_if(iterator b, iterator e, auto &&pred)
    {
      for (; b != e and not pred(*b); ++b)
        ;

      return b;
    }

    template <typename iterator>
    iterator find_if_not(iterator b, iterator e, auto &&pred)
    {
      return find_if(b, e, [&pred](const auto &item) { return not pred(item); });
    }

    template <typename iterator>
    void reverse(iterator b, iterator e)
    {
      while ((b != e) && (b != --e))
      {
        auto tmp = static_cast<decltype(*b) &&>(*b);
        *b = static_cast<decltype(*e) &&>(*e);
        *e = static_cast<decltype(tmp) &&>(tmp);
        b++;
      }
    }

    template <typename iterator>
    void for_each(iterator b, iterator e, auto &&act)
    {
      for (; b != e; ++b)
        act(*b);
    }

    template <typename iterator>
    std::size_t count_if(iterator b, iterator e, auto &&pred)
    {
      std::size_t cnt = 0;
      for (; b != e; ++b)
        if (pred(*b))
          ++cnt;
      return cnt;
    }

    template <typename iterator>
    bool all_of(iterator b, iterator e, auto &&pred)
    {
      return find_if_not(b, e, pred) == e;
    }

    template <typename iterator>
    bool none_of(iterator b, iterator e, auto &&pred)
    {
      return find_if(b, e, pred) == e;
    }

    template <typename iterator>
    bool any_of(iterator b, iterator e, auto &&pred)
    {
      return find_if(b, e, pred) != e;
    }

    template <typename iterator, typename value>
    void replace_if(iterator b, iterator e, auto &&pred, const value &v)
    {
      for (; b != e; ++b)
        if (pred(v))
          *b = v;
    }

    template <typename iterator>
    bool start_with(iterator b, iterator e, iterator bo, iterator eo)
    {
      if (e - b != eo - bo)
        return false;

      for (; b != e and *b == *bo; (++b, ++bo))
        ;

      return bo == eo;
    }
  }

  struct normal_limit
  {
    constexpr std::size_t operator()(std::size_t n) { return n; }
  };

  template <typename type_t, typename end_limit = normal_limit>
  class span
  {
  public:
    type_t *b = nullptr;
    type_t *e = nullptr;

  public:
    constexpr span() noexcept = default;
    constexpr span(const span &s) noexcept = default;
    constexpr span(span &&s) noexcept = default;
    span &operator=(const span &s) noexcept = default;
    span &operator=(span &&s) noexcept = default;
    ~span() = default;
    constexpr explicit span(type_t *_begin, type_t *_end) : b(_begin), e(_end) {}
    constexpr explicit span(type_t *_begin, std::size_t _length) : span(_begin, _begin + _length) {}

    template <std::size_t n>
    constexpr span(const type_t (&_begin)[n]) : span(_begin, end_limit{}(n)) {}

  public:
    constexpr type_t *begin() { return b; }
    constexpr type_t *end() { return e; }
    constexpr const type_t *begin() const { return b; }
    constexpr const type_t *end() const { return e; }
    constexpr const type_t *data() const { return b; }

    constexpr const type_t &operator[](const std::size_t &i) const { return *(b + i); }
    constexpr type_t &operator[](const std::size_t &i) { return *(b + i); }

    constexpr std::size_t size() const { return e - b; }

    constexpr bool operator==(const span &o) const
    {
      return size() == o.size() and start_with(o);
    }

  public:
    span &for_each(auto &&action)
    {
      detail::for_each(begin(), end(), action);
      return *this;
    }

    const span &for_each(auto &&action) const
    {
      detail::for_each(begin(), end(), action);
      return *this;
    }

    span &map(auto &&transform) { return for_each(transform); }

    type_t *find_if(auto &&pred)
    {
      return detail::find_if(begin(), end(), pred);
    }

    const type_t *find_if(auto &&pred) const
    {
      return detail::find_if(begin(), end(), pred);
    }

    type_t *find(const type_t &t)
    {
      return find_if([&t](const type_t &o) { return t == o; });
    }

    const type_t *find(const type_t &t) const
    {
      return find_if([&t](const type_t &o) { return t == o; });
    }

    span &reverse()
    {
      detail::reverse(begin(), end());
      return *this;
    }

    std::size_t count_if(auto &&pred) const
    {
      return detail::count_if(begin(), end(), pred);
    }

    std::size_t count(const type_t &t) const
    {
      return count_if([&t](auto &&item) { return item == t; });
    }

    bool all_of(auto &&pred) const
    {
      return detail::all_of(begin(), end(), pred);
    }

    bool none_of(auto &&pred) const
    {
      return detail::none_of(begin(), end(), pred);
    }

    bool any_of(auto &&pred) const
    {
      return detail::any_of(begin(), end(), pred);
    }

    bool start_with(const span &o) const
    {
      return detail::start_with(begin(), end(), o.begin(), o.end());
    }
  };
}

#endif