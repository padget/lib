#ifndef __lib_array_hpp__
#define __lib_array_hpp__

#include <cstddef>
#include "span.hpp"

namespace lib
{
  template <typename type_t, std::size_t n>
  requires(n > 0) class array
  {

    type_t _data[n];

  public:
    constexpr std::size_t size() const { return n; }
    constexpr bool empty() const { return size() == 0; }
    type_t *data() { return _data; }
    constexpr const type_t *data() const { return _data; }
    type_t *begin() { return _data; }
    type_t *end() { return _data + n; }
    constexpr const type_t *begin() const { return _data; }
    constexpr const type_t *end() const { return _data + n; }
    
    type_t &front() { return *begin(); }
    constexpr const type_t &front() const { return *begin(); }
    type_t &back() { return *(end() - 1); }
    constexpr const type_t &back() const { return *(end() - 1); }

  public:
    type_t &operator[](std::size_t i) { return _data[i]; }
    constexpr const type_t &operator[](std::size_t i) const { return _data[i]; }

  public:
    constexpr bool operator==(const type_t (&a)[n]) const
    {
      return span<const type_t>(a, n) == span<const type_t>(_data, n);
    }
  };
}

#endif