#ifndef __lib_span_hpp__
#define __lib_span_hpp__

#include <cstddef>
#include <lib/utility.hpp>
#include <lib/algorithm.hpp>

namespace lib
{
  struct normal_limit
  {
    constexpr std::size_t
    operator()(
        std::size_t n)
    {
      return n;
    }
  };

  template <
      typename type_t,
      typename end_limit = normal_limit>
  class span
  {
  public:
    type_t *b = nullptr;
    type_t *e = nullptr;

  public:
    constexpr span() noexcept = default;
    constexpr span(const span &s) noexcept = default;
    span &operator=(const span &s) noexcept = default;
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
    constexpr const type_t &operator[](std::size_t i) const { return *(b + i); }
    constexpr type_t &operator[](std::size_t i) { return *(b + i); }
    constexpr std::size_t size() const { return e - b; }
    constexpr bool empty() const { return size() == 0; }
  };
}

#endif