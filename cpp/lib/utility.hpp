#ifndef __lib_utility_hpp__
#define __lib_utility_hpp__

#include <lib/meta.hpp>

namespace lib
{
  using size_t = long unsigned;

  template <typename type_t>
  constexpr remove_reference<type_t> &&
  move(type_t &&t)
  {
    return static_cast<remove_reference<type_t> &&>(t);
  }

  template <typename type_t>
  constexpr void memcpy(
      type_t *src,
      type_t *dest,
      unsigned n)
  {
    char *destp = reinterpret_cast<char *>(dest);
    char *srcp = reinterpret_cast<char *>(src);
    unsigned np = n * sizeof(type_t);

    while (np--)
      *destp++ = *srcp++;
  }

  template <
      unsigned i,
      typename arg_t,
      typename... args_t>
  constexpr auto &get(
      const arg_t &arg,
      const args_t &...args)
  {
    static_assert(i <= sizeof...(args));

    if constexpr (i == 0)
      return arg;
    else
      return get<i - 1>(args...);
  }
}

#endif